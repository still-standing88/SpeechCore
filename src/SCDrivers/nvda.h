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
typedef error_status_t(__stdcall* NvdaController_speakText_t)(const wchar_t* text);
typedef error_status_t(__stdcall* NvdaController_cancelSpeech_t)(void);
typedef error_status_t(__stdcall* NvdaController_speakSsml_t)(const wchar_t* ssml, const SYMBOL_LEVEL symbolLevel, const SPEECH_PRIORITY priority, const boolean asynchronous);
typedef error_status_t(__stdcall* NvdaController_brailleMessage_t)(const wchar_t* message);
typedef error_status_t(__stdcall* NvdaController_setOnSsmlMarkReachedCallback_t)(onSsmlMarkReachedFuncType callback);

class ScreenReaderNVDA : public ScreenReader {
private:
	HMODULE module;
	bool loaded;
	bool Is_Active;
	bool IsSpeaking;

	NvdaController_testIfRunning_t nvdaController_testIfRunning_fn;
	NvdaController_speakText_t nvdaController_speakText_fn;
NvdaController_brailleMessage_t nvdaController_brailleMessage_fn;

	NvdaController_cancelSpeech_t nvdaController_cancelSpeech_fn;
	NvdaController_speakSsml_t nvdaController_speakSsml_fn;
	NvdaController_setOnSsmlMarkReachedCallback_t nvdaController_setOnSsmlMarkReachedCallback_fn;

public:
	ScreenReaderNVDA();
	~ScreenReaderNVDA();

	void init() override;
	void release() override;

	bool is_speaking() override { return IsSpeaking; }
	bool is_running() override;

	bool speak_text(const wchar_t* text, bool interrupt = false) override;
	bool output_text(const wchar_t* text,bool interrupt=false, bool with_ssml = false) override;
	bool output_braille(const wchar_t* text) override;
	bool stop_speech() override;

private:
	static ScreenReaderNVDA* currentInstance;
	static error_status_t __stdcall markReachedCallback(const wchar_t* mark);
};