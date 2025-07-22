#include <string>
#include "jaws.h"

bool fromVariant(VARIANT_BOOL VAR) {
	return (VAR == VARIANT_TRUE) ? true : false;
}

ScreenReaderJaws::ScreenReaderJaws() :
	ScreenReader(L"Jaws", SC_HAS_SPEECH | SC_HAS_BRAILLE),
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

	bool ScreenReaderJaws::speak_text(const wchar_t* text, bool interrupt, bool with_ssml) {
return this->speak_text(text, interrupt);
}

bool ScreenReaderJaws::output_braille(const wchar_t* text) {
    if (this->module != nullptr && text != nullptr) {
        
        std::wstring wstr(text);        
        std::wstring::size_type pos = wstr.find_first_of(L"\"");

        while (pos != std::wstring::npos) {
            wstr[pos] = L'\'';
            pos = wstr.find_first_of(L"\"", pos + 1);
        }
        
        wstr.insert(0, L"BrailleString(\"");
        wstr.append(L"\")");
        

        bstr_t scriptCall = SysAllocString(wstr.c_str());
        VARIANT_BOOL bool_var;
        auto res = this->module->RunFunction(scriptCall, &bool_var);
        SysFreeString(scriptCall);
        
        return (SUCCEEDED(res) && fromVariant(bool_var));
    }
    return false;
}

	bool ScreenReaderJaws::stop_speech() {
		if (this->module != nullptr) {
			return SUCCEEDED(this->module->StopSpeech());
		}
		return false;
}


bool ScreenReaderJaws::run_jaws_function(const wchar_t* functionName) {
    if (this->module != nullptr && functionName != nullptr) {
        bstr_t funcName = SysAllocString(functionName);
        VARIANT_BOOL bool_var;
        auto res = this->module->RunFunction(funcName, &bool_var);
        SysFreeString(funcName);
        return (SUCCEEDED(res) && fromVariant(bool_var));
    }
    return false;
}
