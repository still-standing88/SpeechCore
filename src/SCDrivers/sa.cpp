#include "sa.h"

ScreenReaderSystemAccess::ScreenReaderSystemAccess():
	ScreenReader(L"SystemAccess", SC_HAS_SPEECH),
	loaded(false),Is_Active(false) {

}
ScreenReaderSystemAccess::~ScreenReaderSystemAccess() {
	this->release();
	}
	void ScreenReaderSystemAccess::init() {
		if (!this->loaded) {
			this->loaded = (sa_initialize() == true) ? true : false;
		}
		if (this->loaded && !this->Is_Active) {
			this->is_running();
		}
	}
	void ScreenReaderSystemAccess::release() {
		this->loaded = false;
		this->Is_Active = false;
		sa_cleanup();
	}
	bool ScreenReaderSystemAccess::is_running() {
		this->Is_Active = (sa_is_active() == true) ? true : false;
		return this->Is_Active;
	}
	bool ScreenReaderSystemAccess::speak_text(const wchar_t* text,bool interrupt) {
		return sa_speak(text, interrupt);
	}
	bool ScreenReaderSystemAccess::stop_speech() {
		return sa_silence();
}