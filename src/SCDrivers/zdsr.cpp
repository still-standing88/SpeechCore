#include "zdsr.h"

ScreenReaderZhengdu::ScreenReaderZhengdu() :
	ScreenReader(L"Zhengdu", SC_HAS_SPEECH),
	module(nullptr),Is_Active(false), loaded(false), result(0) {

}
ScreenReaderZhengdu::~ScreenReaderZhengdu() {
	this->release();
	}
void ScreenReaderZhengdu::init() {
	if (!this->loaded || this->module ==nullptr) {
		this->module = new ZDSRAPI();
		this->loaded = this->module->loadLibrary(ZDSR_MODULE);
	}
	if (this->loaded && !this->Is_Active) {
		this->is_running();
	}
}
	void ScreenReaderZhengdu::release() {
		this->loaded = false;
		this->Is_Active = false;
		this->module->unloadLibrary();
		delete this->module;
		this->module = nullptr;
	}
	bool ScreenReaderZhengdu::is_running() {
		if (this->module->InitTTS) {
			this->result = this->module->InitTTS(0, NULL);
			(this->result == 0) ? this->Is_Active = true : this->Is_Active = false;
		}
		this->Is_Active = false;
		return this->Is_Active;
	}
	bool ScreenReaderZhengdu::speak_text(const wchar_t* text,bool interrupt) {
		WCHAR* txt = reinterpret_cast<WCHAR*>(&text);
		if (this->module != nullptr) {
this->result = this->module->Speak(txt, interrupt);
return (this->result == 0) ? true : false;
}
		return false;
	}
	bool ScreenReaderZhengdu::is_speaking() {
		if (this->module != nullptr) {
this->result = this->module->GetSpeakState();
return (this->result == 3) ? true : false;
		}
		return false;
}
	bool ScreenReaderZhengdu::stop_speech() {
		if (this->module != nullptr) {
			this->module->StopSpeak();
			return true;
}
		return false;
}
