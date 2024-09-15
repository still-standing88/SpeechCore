#pragma once
#include <Windows.h>
#include "SCDriver.h"
#include "../ThirdParty/nvdaController.h"
#ifdef _WIN64
#define NVDA_MODULE L"nvdaControllerClient64.dll"
#else
#define NVDA_MODULE L"nvdaControllerClient32.dll"
#endif

typedef error_status_t(__stdcall* NvdaController_testIfRunning_t)(void);
typedef error_status_t(__stdcall* NvdaController_speakText_t)(const wchar_t* text,bool interrupt);
typedef error_status_t(__stdcall* NvdaController_cancelSpeech_t)(void);

class ScreenReaderNVDA : public ScreenReader {
private:
	HMODULE module;
	bool loaded;
	bool Is_Active;
	NvdaController_testIfRunning_t nvdaController_testIfRunning_fn;
	NvdaController_speakText_t nvdaController_speakText_fn;
	NvdaController_cancelSpeech_t nvdaController_cancelSpeech_fn;
public:
	ScreenReaderNVDA();
	~ScreenReaderNVDA();
	void init() override;
	void release() override;
	bool is_speaking() { return false; }
	bool is_running() override;
	bool speak_text(const wchar_t* text, bool interrupt = false) override;
	bool stop_speech() override;
};