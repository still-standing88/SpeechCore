#include "AndroidTTS.h"
#include <unistd.h>

AndroidTTS::AndroidTTS(JNIEnv* environment, jobject context)
    : ScreenReader(L"Android TTS",
        SC_SPEECH_PARAMETER_CONTROL | SC_VOICE_CONFIG | SC_HAS_SPEECH |
        SC_HAS_SPEECH_STATE | SC_SSML_SUPPORT
      env(environment),
      activityContext(context),
      ttsObject(nullptr),
      ttsListener(nullptr),
      currentVoiceIndex(0),
      currentVolume(1.0f),
      currentRate(1.0f),
      currentPitch(1.0f),
      initialized(false),
      ttsReady(false) {
    
    env->GetJavaVM(&jvm);
    activityContext = env->NewGlobalRef(context);
}

AndroidTTS::~AndroidTTS() {
    release();
}

void AndroidTTS::init() {
    if (initialized) return;
    
    LOGI("Initializing Android TTS");
    
    if (!env) {
        int status = jvm->AttachCurrentThread(&env, nullptr);
        if (status != JNI_OK) {
            LOGE("Failed to attach thread to JVM");
            return;
        }
    }
    
    ttsClass = env->FindClass("android/speech/tts/TextToSpeech");
    if (!ttsClass) {
        LOGE("Failed to find TextToSpeech class");
        return;
    }
    ttsClass = (jclass)env->NewGlobalRef(ttsClass);
    
    localeClass = env->FindClass("java/util/Locale");
    localeClass = (jclass)env->NewGlobalRef(localeClass);
    
    voiceClass = env->FindClass("android/speech/tts/Voice");
    voiceClass = (jclass)env->NewGlobalRef(voiceClass);
    
    setClass = env->FindClass("java/util/Set");
    setClass = (jclass)env->NewGlobalRef(setClass);
    
    iteratorClass = env->FindClass("java/util/Iterator");
    iteratorClass = (jclass)env->NewGlobalRef(iteratorClass);
    
    ttsConstructor = env->GetMethodID(ttsClass, "<init>",
        "(Landroid/content/Context;Landroid/speech/tts/TextToSpeech$OnInitListener;)V");
    speakMethod = env->GetMethodID(ttsClass, "speak",
        "(Ljava/lang/CharSequence;ILandroid/os/Bundle;Ljava/lang/String;)I");
    stopMethod = env->GetMethodID(ttsClass, "stop", "()I");
    shutdownMethod = env->GetMethodID(ttsClass, "shutdown", "()V");
    isSpeakingMethod = env->GetMethodID(ttsClass, "isSpeaking", "()Z");
    setPitchMethod = env->GetMethodID(ttsClass, "setPitch", "(F)I");
    setSpeechRateMethod = env->GetMethodID(ttsClass, "setSpeechRate", "(F)I");
    setVolumeMethod = env->GetMethodID(ttsClass, "setVolume", "(F)I");
    getVoicesMethod = env->GetMethodID(ttsClass, "getVoices", "()Ljava/util/Set;");
    setVoiceMethod = env->GetMethodID(ttsClass, "setVoice", "(Landroid/speech/tts/Voice;)I");
    getVoiceMethod = env->GetMethodID(ttsClass, "getVoice", "()Landroid/speech/tts/Voice;");
    
    createTTSListener();
    
    ttsObject = env->NewObject(ttsClass, ttsConstructor, activityContext, ttsListener);
    if (!ttsObject) {
        LOGE("Failed to create TextToSpeech object");
        return;
    }
    ttsObject = env->NewGlobalRef(ttsObject);
    
    initialized = true;
    LOGI("Android TTS initialized successfully");
    
    usleep(1000000);
    loadVoices();
}

void AndroidTTS::release() {
    if (!initialized) return;
    
    LOGI("Releasing Android TTS");
    
    if (ttsObject) {
        env->CallVoidMethod(ttsObject, shutdownMethod);
        env->DeleteGlobalRef(ttsObject);
        ttsObject = nullptr;
    }
    
    if (ttsListener) {
        env->DeleteGlobalRef(ttsListener);
        ttsListener = nullptr;
    }
    
    if (activityContext) {
        env->DeleteGlobalRef(activityContext);
        activityContext = nullptr;
    }
    
    if (ttsClass) env->DeleteGlobalRef(ttsClass);
    if (localeClass) env->DeleteGlobalRef(localeClass);
    if (voiceClass) env->DeleteGlobalRef(voiceClass);
    if (setClass) env->DeleteGlobalRef(setClass);
    if (iteratorClass) env->DeleteGlobalRef(iteratorClass);
    
    initialized = false;
    ttsReady = false;
}

bool AndroidTTS::is_running() {
    return initialized && ttsReady;
}

bool AndroidTTS::is_speaking() {
    if (!is_running()) return false;
    return env->CallBooleanMethod(ttsObject, isSpeakingMethod);
}

bool AndroidTTS::speak_text(const wchar_t* text, bool interrupt) {
    return speak_text(text, interrupt, false);
}

bool AndroidTTS::speak_text(const wchar_t* text, bool interrupt, bool with_ssml) {
    if (!is_running() || !text) return false;
    
    jstring jText = wcharToJString(text);
    if (!jText) return false;
    
    int queueMode = interrupt ? 1 : 0;
    
    int result = env->CallIntMethod(ttsObject, speakMethod, jText, queueMode, nullptr, nullptr);
    
    env->DeleteLocalRef(jText);
    return result == 0;
}

bool AndroidTTS::stop_speech() {
    if (!is_running()) return false;
    int result = env->CallIntMethod(ttsObject, stopMethod);
    return result == 0;
}

float AndroidTTS::get_volume() const {
    return currentVolume;
}

void AndroidTTS::set_volume(float volume) {
    if (!is_running()) return;
    currentVolume = volume;
}

float AndroidTTS::get_rate() const {
    return currentRate;
}

void AndroidTTS::set_rate(float rate) {
    if (!is_running()) return;
    currentRate = rate;
    env->CallIntMethod(ttsObject, setSpeechRateMethod, rate);
}

float AndroidTTS::get_pitch() const {
    return currentPitch;
}

void AndroidTTS::set_pitch(float pitch) {
    if (!is_running()) return;
    currentPitch = pitch;
    env->CallIntMethod(ttsObject, setPitchMethod, pitch);
}

const wchar_t* AndroidTTS::get_voice(int index) const {
    if (index < 0 || index >= voiceNames.size()) return nullptr;
    return voiceNames[index].c_str();
}

void AndroidTTS::set_voice(int index) {
    if (!is_running() || index < 0 || index >= voiceNames.size()) return;
    currentVoiceIndex = index;
}

const wchar_t* AndroidTTS::get_current_voice() const {
    if (currentVoiceIndex >= 0 && currentVoiceIndex < voiceNames.size()) {
        return voiceNames[currentVoiceIndex].c_str();
    }
    return L"";
}

int AndroidTTS::get_voices() const {
    return static_cast<int>(voiceNames.size());
}

void AndroidTTS::createTTSListener() {
    jclass listenerClass = env->FindClass("android/speech/tts/TextToSpeech$OnInitListener");
    jmethodID constructor = env->GetMethodID(listenerClass, "<init>", "()V");
    ttsListener = env->NewObject(listenerClass, constructor);
    ttsListener = env->NewGlobalRef(ttsListener);
}

void AndroidTTS::loadVoices() {
    if (!is_running()) return;
    
    voiceNames.clear();
    
    jobject voicesSet = env->CallObjectMethod(ttsObject, getVoicesMethod);
    if (!voicesSet) return;
    
    jmethodID iteratorMethod = env->GetMethodID(setClass, "iterator", "()Ljava/util/Iterator;");
    jobject iterator = env->CallObjectMethod(voicesSet, iteratorMethod);
    
    jmethodID hasNextMethod = env->GetMethodID(iteratorClass, "hasNext", "()Z");
    jmethodID nextMethod = env->GetMethodID(iteratorClass, "next", "()Ljava/lang/Object;");
    jmethodID getNameMethod = env->GetMethodID(voiceClass, "getName", "()Ljava/lang/String;");
    
    while (env->CallBooleanMethod(iterator, hasNextMethod)) {
        jobject voice = env->CallObjectMethod(iterator, nextMethod);
        jstring voiceName = (jstring)env->CallObjectMethod(voice, getNameMethod);
        
        if (voiceName) {
            std::wstring name = jstringToWString(voiceName);
            voiceNames.push_back(name);
            env->DeleteLocalRef(voiceName);
        }
        env->DeleteLocalRef(voice);
    }
    
    env->DeleteLocalRef(iterator);
    env->DeleteLocalRef(voicesSet);
    
    LOGI("Loaded %d voices", (int)voiceNames.size());
    ttsReady = true;
}

jstring AndroidTTS::wcharToJString(const wchar_t* wstr) {
    if (!wstr) return nullptr;
    
    size_t len = wcslen(wstr);
    std::string utf8Str;
    utf8Str.reserve(len * 4);
    
    for (size_t i = 0; i < len; ++i) {
        wchar_t wc = wstr[i];
        if (wc < 0x80) {
            utf8Str += static_cast<char>(wc);
        } else if (wc < 0x800) {
            utf8Str += static_cast<char>(0xC0 | (wc >> 6));
            utf8Str += static_cast<char>(0x80 | (wc & 0x3F));
        } else if (wc < 0x10000) {
            utf8Str += static_cast<char>(0xE0 | (wc >> 12));
            utf8Str += static_cast<char>(0x80 | ((wc >> 6) & 0x3F));
            utf8Str += static_cast<char>(0x80 | (wc & 0x3F));
        } else {
            utf8Str += static_cast<char>(0xF0 | (wc >> 18));
            utf8Str += static_cast<char>(0x80 | ((wc >> 12) & 0x3F));
            utf8Str += static_cast<char>(0x80 | ((wc >> 6) & 0x3F));
            utf8Str += static_cast<char>(0x80 | (wc & 0x3F));
        }
    }
    
    return env->NewStringUTF(utf8Str.c_str());
}

std::wstring AndroidTTS::jstringToWString(jstring jstr) {
    if (!jstr) return L"";
    
    const char* utf8 = env->GetStringUTFChars(jstr, nullptr);
    if (!utf8) return L"";
    
    size_t len = strlen(utf8);
    std::wstring result;
    result.reserve(len);
    
    for (size_t i = 0; i < len; ++i) {
        unsigned char c = utf8[i];
        if (c < 0x80) {
            result += static_cast<wchar_t>(c);
        } else {
            result += static_cast<wchar_t>(c);
        }
    }
    
    env->ReleaseStringUTFChars(jstr, utf8);
    return result;
}