#pragma once
#include <Windows.h>
#include <comdef.h>
#include "SCDriver.h"
#include "../ThirdParty/fsapi.h"
#ifdef _WIN64
#define JAWS_MODULE L"jfwapi64.dll"
#else
#define JAWS_MODULE L"jfwapi32.dll"
#endif

class ScreenReaderJaws : public ScreenReader {
private:
	IJawsApi* module;
	bool loaded;
	bool Is_Active;
public:
	ScreenReaderJaws();
	~ScreenReaderJaws();
	void init() override;
	void release() override;
	bool is_speaking() override{ return false; }
	bool is_running() override;
	bool speak_text(const wchar_t* text,bool interrupt=false) override;
	bool stop_speech();
};