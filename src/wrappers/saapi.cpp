#include "saapi.h"
#include <windows.h>

static HINSTANCE controller = NULL;

typedef bool (__stdcall *SA_SayW)(const wchar_t *);
typedef bool (__stdcall *SA_BrlShowTextW)(const wchar_t *);
typedef bool (__stdcall *SA_StopAudio)();
typedef bool (__stdcall *SA_IsRunning)();

static SA_SayW sa_SayW = NULL;
static SA_BrlShowTextW sa_BrlShowTextW = NULL;
static SA_StopAudio sa_StopAudio = NULL;
static SA_IsRunning sa_IsRunning = NULL;

bool sa_initialize() {
#ifdef _WIN64
    controller = LoadLibrary(L"SAAPI64.dll");
#else
    controller = LoadLibrary(L"SAAPI32.dll");
#endif

    if (controller) {
        sa_SayW = (SA_SayW)GetProcAddress(controller, "SA_SayW");
        sa_BrlShowTextW = (SA_BrlShowTextW)GetProcAddress(controller, "SA_BrlShowTextW");
        sa_StopAudio = (SA_StopAudio)GetProcAddress(controller, "SA_StopAudio");
        sa_IsRunning = (SA_IsRunning)GetProcAddress(controller, "SA_IsRunning");
        return true;
    }
    return false;
}

void sa_cleanup() {
    if (controller) {
        FreeLibrary(controller);
        controller = NULL;
    }
    sa_SayW = NULL;
    sa_BrlShowTextW = NULL;
    sa_StopAudio = NULL;
    sa_IsRunning = NULL;
}

bool sa_speak(const wchar_t *text, bool interrupt) {
    if (interrupt && !sa_silence()) return false;
    if (sa_SayW) return sa_SayW(text);
    return false;
}

bool sa_braille(const wchar_t *text) {
    if (sa_BrlShowTextW) return sa_BrlShowTextW(text);
    return false;
}

bool sa_silence() {
    if (sa_StopAudio) return sa_StopAudio();
    return false;
}

bool sa_is_active() {
    if (sa_IsRunning) return sa_IsRunning();
    return false;
}

bool sa_output(const wchar_t *text, bool interrupt) {
    bool speak = sa_speak(text, interrupt);
    bool braille = sa_braille(text);
    return (speak || braille);
}