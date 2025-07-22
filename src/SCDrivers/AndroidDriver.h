#pragma once
#include <jni.h>
#include <string>
#include <vector>
#include <android/log.h>
#include "ScreenReader.h"

#define LOG_TAG "AndroidTTS"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

class AndroidTTS : public ScreenReader {
private:
    JNIEnv* env;
    JavaVM* jvm;
    jobject activityContext;
    jobject ttsObject;
    jobject ttsListener;
    
    jmethodID ttsConstructor;
    jmethodID speakMethod;
    jmethodID stopMethod;
    jmethodID shutdownMethod;
    jmethodID isSpeakingMethod;
    jmethodID setPitchMethod;
    jmethodID setSpeechRateMethod;
    jmethodID setVolumeMethod;
    jmethodID getVoicesMethod;
    jmethodID setVoiceMethod;
    jmethodID getVoiceMethod;
    
    jclass ttsClass;
    jclass localeClass;
    jclass voiceClass;
    jclass setClass;
    jclass iteratorClass;
    
    std::vector<std::wstring> voiceNames;
    int currentVoiceIndex;
    
    float currentVolume;
    float currentRate;
    float currentPitch;
    
    bool initialized;
    bool ttsReady;
    
public:
    AndroidTTS(JNIEnv* environment, jobject context);
    virtual ~AndroidTTS();
    
    void init() override;
    void release() override;
    
    bool is_running() override;
    bool is_speaking() override;

    bool speak_text(const wchar_t* text, bool interrupt = false) override;
    bool speak_text(const wchar_t* text, bool interrupt = false, bool with_ssml = false) override;
    bool stop_speech() override;
    
    float get_volume() const override;
    void set_volume(float volume) override;
    float get_rate() const override;
    void set_rate(float rate) override;
    float get_pitch() const override;
    void set_pitch(float pitch) override;
    
    const wchar_t* get_voice(int index) const override;
    void set_voice(int index) override;
    const wchar_t* get_current_voice() const override;
    int get_voices() const override;

private:
    void createTTSListener();
    void loadVoices();
    jstring wcharToJString(const wchar_t* wstr);
    std::wstring jstringToWString(jstring jstr);
};