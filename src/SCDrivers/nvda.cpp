#include "nvda.h"

ScreenReaderNVDA* ScreenReaderNVDA::currentInstance = nullptr;

ScreenReaderNVDA::ScreenReaderNVDA() :
	ScreenReader(L"NVDA", SC_HAS_SPEECH | SC_HAS_SPEECH_STATE | SC_HAS_BRAILLE), module(NULL),
nvdaController_testIfRunning_fn(NULL),
nvdaController_speakText_fn(NULL),
nvdaController_brailleMessage_fn(NULL),
nvdaController_cancelSpeech_fn(NULL),
nvdaController_speakSsml_fn(NULL),
nvdaController_setOnSsmlMarkReachedCallback_fn(NULL),
loaded(false), Is_Active(false), IsSpeaking(false) {
	currentInstance = this;
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
            nvdaController_brailleMessage_fn = (NvdaController_brailleMessage_t)GetProcAddress(this->module, "nvdaController_brailleMessage");

				nvdaController_cancelSpeech_fn = (NvdaController_cancelSpeech_t)GetProcAddress(this ->module, "nvdaController_cancelSpeech");

				nvdaController_speakSsml_fn = (NvdaController_speakSsml_t)GetProcAddress(this->module, "nvdaController_speakSsml");
				nvdaController_setOnSsmlMarkReachedCallback_fn = (NvdaController_setOnSsmlMarkReachedCallback_t)GetProcAddress(this->module, "nvdaController_setOnSsmlMarkReachedCallback");

				if (nvdaController_setOnSsmlMarkReachedCallback_fn) {
					nvdaController_setOnSsmlMarkReachedCallback_fn(this->markReachedCallback);
				}
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
	if (nvdaController_speakSsml_fn && this->nvdaController_setOnSsmlMarkReachedCallback_fn) {
		this->IsSpeaking = true;

		size_t length = wcslen(text);
		wchar_t* ssmlText = new wchar_t[length + 100];
		SPEECH_PRIORITY priority = interrupt ? SPEECH_PRIORITY_NOW : SPEECH_PRIORITY_NORMAL;

		swprintf(ssmlText, L"<speak>%s<mark name='end_of_speech'/></speak>", text);
		auto state = nvdaController_speakSsml_fn(ssmlText, SYMBOL_LEVEL_UNCHANGED, priority, true);
		delete ssmlText;
		return (state == 0) ? true : false;
	}
	else {
		auto state = nvdaController_speakText_fn(text, interrupt);
		return (state == 0) ? true : false;
	}
		}
		return false;
	}

	bool ScreenReaderNVDA::output_text(const wchar_t* text,bool interrupt, bool with_ssml) {
return this->speak_text(text, interrupt);
}

	bool ScreenReaderNVDA::output_braille(const wchar_t* text) {
if (this->module && nvdaController_brailleMessage_fn && text) {
auto state = nvdaController_brailleMessage_fn(text);
return (state == 0) ? true : false;
}
return false;
}

	bool ScreenReaderNVDA::stop_speech() {
		if (this->module) {
			this->IsSpeaking = false;
			auto state = nvdaController_cancelSpeech_fn();
				return (state == 0) ? true : false;
		}
		return false;
}

	error_status_t __stdcall ScreenReaderNVDA::markReachedCallback(const wchar_t* mark) {
		if (wcscmp(mark, L"end_of_speech") == 0 && currentInstance) {
			currentInstance->IsSpeaking = false;
		}
		return 0;
	}