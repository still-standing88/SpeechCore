#pragma once
#ifndef ZDSRAPI_H
#define ZDSRAPI_H

#include <windows.h>

typedef int (WINAPI* InitTTS_t)(int, WCHAR*);
typedef int (WINAPI* Speak_t)(WCHAR*, BOOL);
typedef int (WINAPI* GetSpeakState_t)();
typedef void (WINAPI* StopSpeak_t)();

class ZDSRAPI {
public:
    ZDSRAPI();
    ~ZDSRAPI();
    bool loadLibrary(const wchar_t* dllName);
    void unloadLibrary();
    InitTTS_t InitTTS;
    Speak_t Speak;
    GetSpeakState_t GetSpeakState;
    StopSpeak_t StopSpeak;

private:
    HMODULE hDll;
};

#endif // ZDSRAPI_H
