#define __SPEECH_C_EXPORT
#include <memory>
#include <vector>
#ifdef _WIN32
#include <SDKDDKVer.h>
#include <Windows.h>
#include "./wrappers/SapiSpeech.h"
#endif // _WIN32
#include "../include/SpeechCore.h"
#include "SCDrivers/drivers.h"
#include "SCDrivers/SCDriver.h"

using namespace std;

extern bool IS_LOADED = false;
#ifdef _WIN32
extern bool PREFER_SAPI = false;
extern Sapi5Speech* sapi5 = nullptr;
extern ScreenReaderSapi5* sapi5_driver = nullptr;
#endif // _WIN32
extern ScreenReader* current_driver = nullptr;
vector<ScreenReader*> drivers;

#ifdef _WIN32
extern "C" SPEECH_C_API void Sapi_Init() {
	sapi5 = new Sapi5Speech();
}
extern "C" SPEECH_C_API void Sapi_Release() {
	if (sapi5 != nullptr) {
		delete sapi5;
		sapi5 = nullptr;
	}
}
extern "C" SPEECH_C_API const wchar_t* Sapi_Get_Current_Voice() {
	std::wstring str;
	if (sapi5 != nullptr) {
		return sapi5->get_voice();
	}
	return NULL;
}
extern "C" SPEECH_C_API const wchar_t* Sapi_Get_Voice(int index) {
	if (sapi5 != nullptr) {
		return sapi5->get_voice_by_index(index);
	}
	return NULL;
}
extern "C" SPEECH_C_API void Sapi_Set_Voice(const wchar_t* voice) {
	if (sapi5 != nullptr) {
		sapi5->set_voice(std::wstring(voice));
	}
}
extern "C" SPEECH_C_API void Sapi_Set_Voice_By_Index(int index) {
	if (sapi5 != nullptr) {
		sapi5->set_voice_by_index(index);
	}
}
extern "C" SPEECH_C_API int Sapi_Get_Voices() {
	return (sapi5 != nullptr) ? sapi5->get_voices() : 0;
}
extern "C" SPEECH_C_API float Sapi_Voice_Get_Volume() {
	return (sapi5 != nullptr) ? static_cast<float> (sapi5->get_volume()) : -1;
}
extern "C" SPEECH_C_API void Sapi_Voice_Set_Volume(float volume) {
	if (sapi5 != nullptr) {
		auto value = static_cast<USHORT>(volume);
		sapi5->set_volume(value);
	}
}
extern "C" SPEECH_C_API float Sapi_Voice_Get_Rate() {
	return (sapi5 != nullptr) ? static_cast<float> (sapi5->get_rate()) : -1;
}
extern "C" SPEECH_C_API void Sapi_Voice_Set_Rate(float rate) {
	if (sapi5 != nullptr) {
		auto value = static_cast<long>(rate);
		sapi5->set_rate(value);
	}
}
extern "C" SPEECH_C_API void Sapi_Speak(const wchar_t* text, bool _interrupt, bool _xml) {
	if (sapi5 != nullptr) {
		sapi5->speak_text(text, _interrupt, _xml);
	}
}
extern "C" SPEECH_C_API void Sapi_Output_File(const char* filename, const wchar_t* text, bool _xml) {
	sapi5->speak_to_file(filename, text, _xml);
}

extern "C" SPEECH_C_API void Sapi_Pause() {
	if (sapi5 != nullptr) {
		sapi5->pause_speach();
	}
}
extern "C" SPEECH_C_API void Sapi_Resume() {
	if (sapi5 != nullptr) {
		sapi5->resume_speach();
	}
}
extern "C" SPEECH_C_API void Sapi_Stop() {
	if (sapi5 != nullptr) {
		sapi5->stop_speach();
	}
}
#endif // _WIN32

extern "C" SPEECH_C_API void Speech_Init() {
#ifdef _WIN32
	Sapi_Init();
	ScreenReaderNVDA* nvda_driver(new ScreenReaderNVDA());
	ScreenReaderJaws* jaws_driver(new ScreenReaderJaws());
	ScreenReaderSystemAccess* sa_driver(new ScreenReaderSystemAccess());
	sapi5_driver = new ScreenReaderSapi5(sapi5);
	ScreenReaderZhengdu* zdsr_driver(new ScreenReaderZhengdu());
	nvda_driver->init();
	jaws_driver->init();
	sa_driver->init();
	zdsr_driver->init();
	drivers.push_back(nvda_driver);
	drivers.push_back(jaws_driver);
	//drivers.push_back(sapi5_driver);
	drivers.push_back(sa_driver);
	drivers.push_back(zdsr_driver);
#elif defined(__APPLE__)
	AVTTSVoiceDriver* av_tts = new AVTTSVoiceDriver();
	av_tts->init();
	drivers.push_back(av_tts);
#elif defined(__linux__) || defined(__unix__)
	SpeechDispatcher* speech_dispatcher = new SpeechDispatcher();
	speech_dispatcher->init();
	drivers.push_back(speech_dispatcher);
#endif // _WIN32
	

	Speech_Detect_Driver();
	IS_LOADED = true;
}
extern "C" SPEECH_C_API void Speech_Free() {
	if (!drivers.empty()) {
		/*auto ittr_driver = drivers.begin();
		while (ittr_driver != drivers.end()) {
			auto driver = *(ittr_driver);
			driver->release();
			//ittr_driver = drivers.erase(ittr_driver);
		}*/
		drivers.clear();
}

#ifdef _WIN32
	//Sapi_Release();
	sapi5_driver->release();
	sapi5_driver = nullptr;
#endif // _WIN32

	IS_LOADED = false;
	current_driver = nullptr;
}

void set_driver() {
	for (auto driver : drivers) {
		if (driver->is_running()) {
			current_driver = driver;
		}
	}
}

extern "C" SPEECH_C_API void Speech_Detect_Driver() {
	if (current_driver == nullptr) {
#ifdef _WIN32
		if (PREFER_SAPI) {
			current_driver = sapi5_driver;
		}
		else {
			set_driver();
		}
#else
		set_driver();
#endif // _WIN32
		if (current_driver == nullptr) {
#ifdef _WIN32
			current_driver = sapi5_driver;
#else
set_driver();
#endif // _WIN32

		}
	}
	else if (current_driver != nullptr) {
		if (!current_driver->is_running()) {
			set_driver();
}
	}
}
#ifdef _WIN32
extern "C" SPEECH_C_API void Speech_Prefer_Sapi(bool prefer_sapi) {
	PREFER_SAPI = prefer_sapi;
}
extern "C" SPEECH_C_API bool Speech_Sapi_loaded() {
	return (sapi5 != nullptr && sapi5->is_active());
}

#endif // _WIN32

extern "C" SPEECH_C_API bool Speech_Is_speaking() {
	if (current_driver != nullptr && !current_driver->is_running()) {
		return current_driver->is_speaking();
	}
	return false;
}

extern "C" SPEECH_C_API const wchar_t* Speech_Current_Driver() {
	if (current_driver != nullptr &&current_driver->is_running()) {
		
		return current_driver->get_name();
	}
	return L"";
}

extern "C" SPEECH_C_API const wchar_t* Speech_Get_Driver(int index) {
	return (!drivers.empty() && index>=0 && index < static_cast<int> (drivers.size())) ? drivers[index]->get_name() : L"";
}

extern "C" SPEECH_C_API void Speech_Set_Driver(int index) {
	if (!drivers.empty() && index>=0 && index < static_cast<int> (drivers.size())) {
		current_driver = drivers[index];
}
}

extern "C" SPEECH_C_API int Speech_Get_Drivers() {
	int length = static_cast<int> (drivers.size());
	return (!drivers.empty() ) ? length : 0;
}

extern "C" SPEECH_C_API bool Speech_Is_Loaded() {
	return IS_LOADED;
}
extern "C" SPEECH_C_API bool Speech_Output(const wchar_t* text, bool _interrupt) {
	if (current_driver == nullptr) {
		Speech_Detect_Driver();
	}
	if (current_driver != nullptr && current_driver->is_running() && text) {
		return current_driver->speak_text(text, _interrupt);
	}
	else {
		Speech_Detect_Driver();
		if (current_driver != nullptr && !current_driver->is_running() && text) {
			return current_driver->speak_text(text, _interrupt);
}
		return false;
	}
	return false;
}

extern "C" SPEECH_C_API bool Speech_Braille(const wchar_t* text) {
	if (current_driver == nullptr) {
		Speech_Detect_Driver();
	}
	if (current_driver != nullptr && current_driver->is_running() && text) {
		return (current_driver->get_speech_flags() & SC_HAS_BRAILLE) ? current_driver->output_braille(text) : false;
	}
	else {
		Speech_Detect_Driver();
		if (current_driver != nullptr && !current_driver->is_running() && text) {
			return (current_driver->get_speech_flags() & SC_HAS_BRAILLE) ? current_driver->output_braille(text) : false;
		}
		return false;
	}
	return false;
}

extern "C" SPEECH_C_API bool Speech_Stop() {
	if (current_driver != nullptr) {
		return current_driver->stop_speech();
	}
	return false;
}
extern "C" SPEECH_C_API float Speech_Get_Volume() {
	return (current_driver != nullptr) ? current_driver->get_volume() : -1;
}
extern "C" SPEECH_C_API void Speech_Set_Volume(float offset) {
	if (current_driver != nullptr && offset >=0) {
		current_driver->set_volume(offset);
	}
}
extern "C" SPEECH_C_API float Speech_Get_Rate() {
	return (current_driver != nullptr) ? current_driver->get_rate() : -1;
}
extern "C" SPEECH_C_API void Speech_Set_Rate(float offset) {
	if (current_driver != nullptr && offset >=0 ) {
		current_driver->set_rate(offset);
	}
}
extern "C" SPEECH_C_API const wchar_t* Speech_Get_Current_Voice() {
	return (current_driver != nullptr) ? current_driver->get_current_voice() : NULL;
}
extern "C" SPEECH_C_API const wchar_t* Speech_Get_Voice(int index) {
	return (current_driver != nullptr && index >= 0) ? current_driver->get_voice(index) : NULL;
}
extern "C" SPEECH_C_API void Speech_Set_voice(int index) {
	if (current_driver != nullptr && index >= 0) {
		current_driver->set_voice(index);
	}
}
extern "C" SPEECH_C_API int Speech_Get_Voices() {
	return (current_driver != nullptr) ? current_driver->get_voices() : 0;
}
extern "C" SPEECH_C_API void Speech_Output_File(const char* filePath, const wchar_t* text) {
	if (current_driver != nullptr && filePath && text) {
		current_driver->output_file(filePath, text);
	}
}
extern "C" SPEECH_C_API void Speech_Resume() {
	if (current_driver != nullptr) {
		current_driver->resume_speech();
	}
}
extern "C" SPEECH_C_API void Speech_pause() {
	if (current_driver != nullptr) {
		current_driver->pause_speech();
	}
}
extern "C" SPEECH_C_API uint32_t Get_Speech_Flags() {
	return (current_driver != nullptr) ? current_driver->get_speech_flags() : 0;
}
