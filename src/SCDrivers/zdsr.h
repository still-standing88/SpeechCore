#pragma once
#include "SCDriver.h"
#include "../wrappers/zdsrapi.h"
#include <cwchar>
#ifdef _WIN64
#define ZDSR_MODULE L"ZDSRAPI_x64.dll"
#else
#define ZDSR_MODULE L"ZDSRAPI.dll"
#endif

class ScreenReaderZhengdu : public ScreenReader {
private:
	ZDSRAPI *module;
	int result;
	bool loaded;
	bool Is_Active;
public:
	ScreenReaderZhengdu();
	~ScreenReaderZhengdu();
	void init() override;
	void release() override;
	bool is_speaking() override;
	bool is_running() override;
	bool speak_text(const wchar_t* _text,bool interrupt=false) override;
	bool stop_speech() override;
	int is_active() { return this->result; }
};