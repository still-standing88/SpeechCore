#include "jaws.h"

bool fromVariant(VARIANT_BOOL VAR) {
	return (VAR == VARIANT_TRUE) ? true : false;
}

ScreenReaderJaws::ScreenReaderJaws() :
	ScreenReader(L"Jaws", SC_HAS_SPEECH),
	module(nullptr),Is_Active(false) {

}
ScreenReaderJaws::~ScreenReaderJaws() {
	this->release();
	}
	void ScreenReaderJaws::init() {
		auto result = CoInitializeEx(NULL, COINIT_MULTITHREADED);
		if (FAILED(result)) {
			CoUninitialize();
		}
		if (!this->loaded || this->module == nullptr) {
			auto res = CoCreateInstance(CLSID_JawsApi, NULL, CLSCTX_ALL, IID_IJawsApi, (void**)&this->module);
			if (SUCCEEDED(res)) {
				this->loaded = true;
}
			if (this->loaded && !this->Is_Active) {
				this->is_running();
}
		}
	}
	void ScreenReaderJaws::release() {
		this->loaded = false;
		this->Is_Active = false;
		if (this->module != nullptr) {
			VARIANT_BOOL bool_var;
			this->module->Disable(&bool_var);
			this->module->Release();
			this->module = nullptr;
			CoUninitialize();
		}
	}
	bool ScreenReaderJaws::is_running() {
		if (this->module != nullptr) {
			VARIANT_BOOL bool_var;
			auto res = this->module->Enable(VARIANT_FALSE, &bool_var);
			(fromVariant(bool_var) == true && SUCCEEDED(res)) ? this->Is_Active = true : this->Is_Active = false;
			return this->Is_Active;
		}
		return false;
	}
	bool ScreenReaderJaws::speak_text(const wchar_t* text, bool interrupt) {
		if (this->module != nullptr) {
			bstr_t str = SysAllocString(text);
VARIANT_BOOL bool_var;
VARIANT_BOOL interrupt_speach = (interrupt == true) ? VARIANT_TRUE : VARIANT_FALSE;
auto res = this->module->SayString(str,interrupt_speach,&bool_var);
			SysFreeString(str);
			return (SUCCEEDED(res) && fromVariant(bool_var) == true);
		}
		return false;
	}
	bool ScreenReaderJaws::stop_speech() {
		if (this->module != nullptr) {
			return SUCCEEDED(this->module->StopSpeech());
		}
		return false;
}
