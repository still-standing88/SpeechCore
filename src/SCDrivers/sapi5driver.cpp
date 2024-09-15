#include "sapi5driver.h"

ScreenReaderSapi5::ScreenReaderSapi5(Sapi5Speech *sapi_instance):
	ScreenReader(L"Sapi5",SC_VOICE_CONFIG|SC_SPEECH_FLOW_CONTROL|SC_SPEECH_PARAMETER_CONTROL|SC_FILE_OUTPUT),
	loaded(false),Is_Active(false),module(sapi_instance) {

}
ScreenReaderSapi5::~ScreenReaderSapi5() {
	this->release();
	}
	void ScreenReaderSapi5::init() {
		if (!this->loaded) {
			loaded = true;
		}
		if (this->loaded && !this->Is_Active) {
			this->is_running();
		}
	}
	void ScreenReaderSapi5::release() {
		if (this->module != nullptr) {
			this->loaded = false;
			this->Is_Active = false;
			delete this->module;
			this->module = nullptr;
		}
	}
	bool ScreenReaderSapi5::is_running() {
		if (this->module != nullptr) {
			this->Is_Active = (this->module==NULL) ? false : true;
			return this->Is_Active;
		}
		return false;
	}
	bool ScreenReaderSapi5::is_speaking() {
		if (this->module != nullptr) {
			return this->module->is_speaking();
		}
		return false;
	}
	bool ScreenReaderSapi5::speak_text(const wchar_t* text,bool interrupt) {
		if (this->module != nullptr) {
			this->module->speak_text(text, interrupt);
			return true;
		}
		return false;
	}
	bool ScreenReaderSapi5::stop_speech() {
		if (this->module != nullptr) {
			this->module->stop_speach();
			return true;
		}
		return false;
}
	float ScreenReaderSapi5::get_volume() const {
		if (this->module != nullptr) {
			return static_cast<float> (this->module->get_volume());
}
		return -1;
	}

	void ScreenReaderSapi5::set_volume(float offset) {
		if (this->module != nullptr) {
			this->module->set_volume(static_cast<USHORT> (offset));
		}
	}

	float ScreenReaderSapi5::get_rate() const {
		if (this->module != nullptr) {
			return static_cast<float> (this->module->get_rate());
		}
		return -1;
	}

	void ScreenReaderSapi5::set_rate(float offset) {
		if (this->module != nullptr) {
			this->module->set_rate(static_cast<long> (offset));
		}
	}

	const wchar_t* ScreenReaderSapi5::get_voice(int index) const {
		if (this->module != nullptr) {
			return this->module->get_voice_by_index(index);
		}
		return L"";
	}

	const wchar_t* ScreenReaderSapi5::get_current_voice() const {
		if (this->module != nullptr) {
			return this->module->get_voice();
		}
		return L"";
	}


	void ScreenReaderSapi5::set_voice(int index) {
		if (this->module != nullptr) {
			this->module->set_voice_by_index(index);
		}
	}

	int ScreenReaderSapi5::get_voices() const {
		if (this->module != nullptr) {
			return this->module->get_voices();
		}
		return 0;
	}

	void ScreenReaderSapi5::resume_speech() {
		if (this->module != nullptr) {
			this->module->resume_speach();
		}
	}

	void ScreenReaderSapi5::pause_speech() {
		if (this->module != nullptr) {
			this->module->pause_speach();
		}
	}
	void ScreenReaderSapi5::output_file(const char* filePath, const wchar_t* text) {
		this->module->speak_to_file(filePath, text);
	}