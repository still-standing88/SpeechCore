#ifdef __WITH_JAVA
#include <jni.h>
#include <string>
#include <cstdint>
#include <vector>
#include <cstring> 
#include "../include/SpeechCore.h"
#include "SpeechCore_JNI.h"

extern bool IS_LOADED;
extern bool PREFER_SAPI;
#ifdef _WIN32
class Sapi5Speech;  // Forward declaration
extern Sapi5Speech* sapi5;
class ScreenReaderSapi5;  // Forward declaration
extern ScreenReaderSapi5* sapi5_driver;
#endif // _WIN32
class ScreenReader;  // Forward declaration
extern ScreenReader* current_driver;
extern std::vector<ScreenReader*> drivers;
extern "C" {
    JNIEXPORT void JNICALL Java_SpeechCore_Speech_1Init(JNIEnv*, jobject) {
        Speech_Init();
    }

    JNIEXPORT void JNICALL Java_SpeechCore_Speech_1Free(JNIEnv*, jobject) {
        Speech_Free();
    }

    JNIEXPORT void JNICALL Java_SpeechCore_Speech_1Detect_1Driver(JNIEnv*, jobject) {
        Speech_Detect_Driver();
    }

    JNIEXPORT jstring JNICALL Java_SpeechCore_Speech_1Current_1Driver(JNIEnv* env, jobject) {
    const wchar_t* driver = Speech_Current_Driver();
    return env->NewString(reinterpret_cast<const jchar*>(driver), wcslen(driver));
    }
    JNIEXPORT jstring JNICALL Java_SpeechCore_Speech_1Get_1Driver(JNIEnv* env, jobject, jint index) {
        const wchar_t* driver = Speech_Get_Driver(static_cast<int>(index));
        return env->NewString(reinterpret_cast<const jchar*>(driver), wcslen(driver));
    }

    JNIEXPORT void JNICALL Java_SpeechCore_Speech_1Set_1Driver(JNIEnv*, jobject, jint index) {
        Speech_Set_Driver(static_cast<int>(index));
    }

    JNIEXPORT jint JNICALL Java_SpeechCore_Speech_1Get_1Drivers(JNIEnv*, jobject) {
        return static_cast<jint>(Speech_Get_Drivers());
    }

    JNIEXPORT jint JNICALL Java_SpeechCore_Speech_1Get_1Flags(JNIEnv*, jobject) {
        return static_cast<jint>(Speech_Get_Flags());
    }

    JNIEXPORT jboolean JNICALL Java_SpeechCore_Speech_1Is_1Loaded(JNIEnv*, jobject) {
        return static_cast<jboolean>(Speech_Is_Loaded());
    }

    JNIEXPORT jboolean JNICALL Java_SpeechCore_Speech_1Is_1Speaking(JNIEnv*, jobject) {
        return static_cast<jboolean>(Speech_Is_Speaking());
    }

    JNIEXPORT jboolean JNICALL Java_SpeechCore_Speech_1Output(JNIEnv* env, jobject, jstring text, jboolean _interrupt) {
    const jchar* jtext = env->GetStringChars(text, nullptr);
    if (jtext == nullptr) {
        return JNI_FALSE;
    }
    const wchar_t* wtext = reinterpret_cast<const wchar_t*>(jtext);
    bool result = Speech_Output(wtext, static_cast<bool>(_interrupt));
    env->ReleaseStringChars(text, reinterpret_cast<const jchar*>(wtext));
    return static_cast<jboolean>(result);
    }
    JNIEXPORT jboolean JNICALL Java_SpeechCore_Speech_1Braille(JNIEnv* env, jobject, jstring text) {
        const jchar* jtext = env->GetStringChars(text, nullptr);
        if (jtext == nullptr) {
            return JNI_FALSE; // String conversion failed
        }
        const wchar_t* wtext = reinterpret_cast<const wchar_t*>(jtext);
        bool result = Speech_Braille(wtext);
        env->ReleaseStringChars(text, reinterpret_cast<const jchar*>(wtext));
        return static_cast<jboolean>(result);
    }

    JNIEXPORT jboolean JNICALL Java_SpeechCore_Speech_1Stop(JNIEnv*, jobject) {
        return static_cast<jboolean>(Speech_Stop());
    }

    JNIEXPORT jfloat JNICALL Java_SpeechCore_Speech_1Get_1Volume(JNIEnv*, jobject) {
        return static_cast<jfloat>(Speech_Get_Volume());
    }

    JNIEXPORT void JNICALL Java_SpeechCore_Speech_1Set_1Volume(JNIEnv*, jobject, jfloat volume) {
        Speech_Set_Volume(static_cast<float>(volume));
    }

    JNIEXPORT jfloat JNICALL Java_SpeechCore_Speech_1Get_1Rate(JNIEnv*, jobject) {
        return static_cast<jfloat>(Speech_Get_Rate());
    }

    JNIEXPORT void JNICALL Java_SpeechCore_Speech_1Set_1Rate(JNIEnv*, jobject, jfloat rate) {
        Speech_Set_Rate(static_cast<float>(rate));
    }

JNIEXPORT jstring JNICALL Java_SpeechCore_Speech_1Get_1Current_1Voice(JNIEnv* env, jobject) {
    const wchar_t* voice = Speech_Get_Current_Voice();
    return env->NewString(reinterpret_cast<const jchar*>(voice), wcslen(voice));
}

    JNIEXPORT void JNICALL Java_SpeechCore_Speech_1Set_1Voice(JNIEnv*, jobject, jint index) {
        Speech_Set_Voice(static_cast<int>(index));
    }
    JNIEXPORT jstring JNICALL Java_SpeechCore_Speech_1Get_1Voice(JNIEnv* env, jobject, jint index) {
        const wchar_t* voice = Speech_Get_Voice(static_cast<int>(index));
        return env->NewString(reinterpret_cast<const jchar*>(voice), wcslen(voice));
    }

    JNIEXPORT jint JNICALL Java_SpeechCore_Speech_1Get_1Voices(JNIEnv*, jobject) {
        return static_cast<jint>(Speech_Get_Voices());
    }

JNIEXPORT void JNICALL Java_SpeechCore_Speech_1Output_1File(JNIEnv* env, jobject, jstring filePath, jstring text) {
    const char* cfilePath = env->GetStringUTFChars(filePath, nullptr);
    if (cfilePath == nullptr) {
        return; // String conversion failed
    }
    const jchar* jtext = env->GetStringChars(text, nullptr);
    if (jtext == nullptr) {
        env->ReleaseStringUTFChars(filePath, cfilePath);
        return; // String conversion failed
    }
    const wchar_t* wtext = reinterpret_cast<const wchar_t*>(jtext);
    Speech_Output_File(cfilePath, wtext);
    env->ReleaseStringUTFChars(filePath, cfilePath);
    env->ReleaseStringChars(text, reinterpret_cast<const jchar*>(wtext));
}

    JNIEXPORT void JNICALL Java_SpeechCore_Speech_1Resume(JNIEnv*, jobject) {
        Speech_Resume();
    }

    JNIEXPORT void JNICALL Java_SpeechCore_Speech_1Pause(JNIEnv*, jobject) {
        Speech_Pause();
    }

#ifdef _WIN32
    JNIEXPORT void JNICALL Java_SpeechCore_Speech_1Prefer_1Sapi(JNIEnv*, jobject, jboolean prefer_sapi) {
        Speech_Prefer_Sapi(static_cast<bool>(prefer_sapi));
    }

    JNIEXPORT jboolean JNICALL Java_SpeechCore_Speech_1Sapi_1Loaded(JNIEnv*, jobject) {
        return static_cast<jboolean>(Speech_Sapi_Loaded());
    }

    JNIEXPORT void JNICALL Java_SpeechCore_Sapi_1Init(JNIEnv*, jobject) {
        Sapi_Init();
    }

    JNIEXPORT void JNICALL Java_SpeechCore_Sapi_1Release(JNIEnv*, jobject) {
        Sapi_Release();
    }

JNIEXPORT jstring JNICALL Java_SpeechCore_Sapi_1Get_1Current_1Voice(JNIEnv* env, jobject) {
    const wchar_t* voice = Sapi_Get_Current_Voice();
    return env->NewString(reinterpret_cast<const jchar*>(voice), wcslen(voice));
}

JNIEXPORT jstring JNICALL Java_SpeechCore_Sapi_1Get_1Voice(JNIEnv* env, jobject, jint index) {
    const wchar_t* voice = Sapi_Get_Voice(static_cast<int>(index));
    return env->NewString(reinterpret_cast<const jchar*>(voice), wcslen(voice));
}

JNIEXPORT void JNICALL Java_SpeechCore_Sapi_1Set_1Voice(JNIEnv* env, jobject, jstring voice) {
    const jchar* jvoice = env->GetStringChars(voice, nullptr);
    if (jvoice == nullptr) {
        return; // String conversion failed
    }
    const wchar_t* wvoice = reinterpret_cast<const wchar_t*>(jvoice);
    Sapi_Set_Voice(wvoice);
    env->ReleaseStringChars(voice, reinterpret_cast<const jchar*>(wvoice));
}

    JNIEXPORT void JNICALL Java_SpeechCore_Sapi_1Set_1Voice_1By_1Index(JNIEnv*, jobject, jint index) {
        Sapi_Set_Voice_By_Index(static_cast<int>(index));
    }

    JNIEXPORT jint JNICALL Java_SpeechCore_Sapi_1Get_1Voices(JNIEnv*, jobject) {
        return static_cast<jint>(Sapi_Get_Voices());
    }

    JNIEXPORT jfloat JNICALL Java_SpeechCore_Sapi_1Voice_1Get_1Volume(JNIEnv*, jobject) {
        return static_cast<jfloat>(Sapi_Voice_Get_Volume());
    }

    JNIEXPORT void JNICALL Java_SpeechCore_Sapi_1Voice_1Set_1Volume(JNIEnv*, jobject, jfloat volume) {
        Sapi_Voice_Set_Volume(static_cast<float>(volume));
    }

    JNIEXPORT jfloat JNICALL Java_SpeechCore_Sapi_1Voice_1Get_1Rate(JNIEnv*, jobject) {
        return static_cast<jfloat>(Sapi_Voice_Get_Rate());
    }

    JNIEXPORT void JNICALL Java_SpeechCore_Sapi_1Voice_1Set_1Rate(JNIEnv*, jobject, jfloat rate) {
        Sapi_Voice_Set_Rate(static_cast<float>(rate));
    }

JNIEXPORT void JNICALL Java_SpeechCore_Sapi_1Speak(JNIEnv* env, jobject, jstring text, jboolean _interrupt, jboolean _xml) {
    const jchar* jtext = env->GetStringChars(text, nullptr);
    if (jtext == nullptr) {
        return; // String conversion failed
    }
    const wchar_t* wtext = reinterpret_cast<const wchar_t*>(jtext);
    Sapi_Speak(wtext, static_cast<bool>(_interrupt), static_cast<bool>(_xml));
    env->ReleaseStringChars(text, reinterpret_cast<const jchar*>(wtext));
}

JNIEXPORT void JNICALL Java_SpeechCore_Sapi_1Output_1File(JNIEnv* env, jobject, jstring filename, jstring text, jboolean _xml) {
    const char* cfilename = env->GetStringUTFChars(filename, nullptr);
    if (cfilename == nullptr) {
        return; // String conversion failed
    }
    const jchar* jtext = env->GetStringChars(text, nullptr);
    if (jtext == nullptr) {
        env->ReleaseStringUTFChars(filename, cfilename);
        return; // String conversion failed
    }
    const wchar_t* wtext = reinterpret_cast<const wchar_t*>(jtext);
    Sapi_Output_File(cfilename, wtext, static_cast<bool>(_xml));
    env->ReleaseStringUTFChars(filename, cfilename);
    env->ReleaseStringChars(text, reinterpret_cast<const jchar*>(wtext));
}
    JNIEXPORT void JNICALL Java_SpeechCore_Sapi_1Pause(JNIEnv*, jobject) {
        Sapi_Pause();
    }

    JNIEXPORT void JNICALL Java_SpeechCore_Sapi_1Resume(JNIEnv*, jobject) {
        Sapi_Resume();
    }

    JNIEXPORT void JNICALL Java_SpeechCore_Sapi_1Stop(JNIEnv*, jobject) {
        Sapi_Stop();
    }
#endif // _WIN32
}
#endif // __WITH_JAVA
