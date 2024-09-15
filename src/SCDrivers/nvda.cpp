#include "nvda.h"

ScreenReaderNVDA::ScreenReaderNVDA() :
	ScreenReader(L"NVDA", SC_HAS_SPEECH),module(NULL),
nvdaController_testIfRunning_fn(NULL),
nvdaController_speakText_fn(NULL),
nvdaController_cancelSpeech_fn(NULL),
loaded(false), Is_Active(false) {
}
ScreenReaderNVDA::~ScreenReaderNVDA() {
	this->release();
	}
	void ScreenReaderNVDA::init() {

		if (!this->loaded) {
			this->module = LoadLibrary(NVDA_MODULE);
			if (this->module) {
				nvdaController_testIfRunning_fn = (NvdaController_testIfRunning_t)GetProcAddress(this->module, "nvdaController_testIfRunning");
				nvdaController_speakText_fn = (NvdaController_speakText_t)GetProcAddress(this->module, "nvdaController_speakText");
				nvdaController_cancelSpeech_fn = (NvdaController_cancelSpeech_t)GetProcAddress(this ->module, "nvdaController_cancelSpeech");
				this->loaded = true;
}
			if (this->loaded && !this->Is_Active) {
				this->is_running();
			}
}
	}
	void ScreenReaderNVDA::release() {
		if (this->module) {
			this->loaded = false;
			this->Is_Active = false;
			FreeLibrary(this->module);
			this->module = NULL;
		}
	}
	bool ScreenReaderNVDA::is_running() {
		if (this->module) {
			return (nvdaController_testIfRunning_fn() == 0) ? true : false;
}
		return false;
	}
	bool ScreenReaderNVDA::speak_text(const wchar_t* text,bool interrupt) {
if (this->module) {
	auto state = nvdaController_speakText_fn(text,interrupt);
	return (state == 0) ? true : false;
		}
		return false;
	}
	bool ScreenReaderNVDA::stop_speech() {
		if (this->module) {
			auto state = nvdaController_cancelSpeech_fn();
				return (state == 0) ? true : false;
		}
		return false;
}