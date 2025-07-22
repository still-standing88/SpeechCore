#pragma once
#include <Windows.h>
#include <string>
#include "SCDriver.h"

#ifdef _WIN64
#define PCTALKER_MODULE L"pctkusr64.dll"
#else
#define PCTALKER_MODULE L"pctkusr.dll"
#endif

typedef int(__stdcall* PCTKStatus_t)(void);
typedef int(__stdcall* PCTKPRead_t)(const char* text, int param1, int param2);
typedef int(__stdcall* PCTKVReset_t)(void);

class ScreenReaderPCTalker : public ScreenReader {
private:
    HMODULE module;
    bool loaded;
    bool Is_Active;
    bool IsSpeaking;
    
    PCTKStatus_t pctk_status_fn;
    PCTKPRead_t pctk_read_fn;
    PCTKVReset_t pctk_reset_fn;

public:
    ScreenReaderPCTalker();
    ~ScreenReaderPCTalker();

    void init() override;
    void release() override;
    bool is_running() override;
	bool is_speaking() override { return false; }
    bool speak_text(const wchar_t* text, bool interrupt = false) override;
	bool speak_text(const wchar_t* text, bool interrupt = false, bool with_ssml= false) override;
    bool stop_speech() override;

private:
    std::string wstring_to_cp932(const wchar_t* wstr);
};