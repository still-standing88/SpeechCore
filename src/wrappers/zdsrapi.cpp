#include "ZDSRAPI.h"

ZDSRAPI::ZDSRAPI() : hDll(nullptr), InitTTS(nullptr), Speak(nullptr), GetSpeakState(nullptr), StopSpeak(nullptr) {}

ZDSRAPI::~ZDSRAPI() {
    unloadLibrary();
}

bool ZDSRAPI::loadLibrary(const wchar_t* dllName) {
    hDll = LoadLibrary(dllName);
    if (!hDll) {
        return false;
    }

    InitTTS = (InitTTS_t)GetProcAddress(hDll, "InitTTS");
    Speak = (Speak_t)GetProcAddress(hDll, "Speak");
    GetSpeakState = (GetSpeakState_t)GetProcAddress(hDll, "GetSpeakState");
    StopSpeak = (StopSpeak_t)GetProcAddress(hDll, "StopSpeak");

    if (!InitTTS || !Speak || !GetSpeakState || !StopSpeak) {

        unloadLibrary();
        return false;
    }
    return true;
}

void ZDSRAPI::unloadLibrary() {
    if (hDll) {
        FreeLibrary(hDll);
        hDll = nullptr;
        InitTTS = nullptr;
        Speak = nullptr;
        GetSpeakState = nullptr;
        StopSpeak = nullptr;
    }
}