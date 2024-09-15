#pragma once
#include "SCDriver.h"
#include "../wrappers/saapi.h"

class ScreenReaderSystemAccess : public ScreenReader {
private:
	bool loaded;
	bool Is_Active;
public:
	ScreenReaderSystemAccess();
	~ScreenReaderSystemAccess();
	void init() override;
	void release() override;
	bool is_speaking()  override{ return false; }
	bool is_running() override;
	bool speak_text(const wchar_t* text, bool interrupt) override;
	bool stop_speech() override;
};