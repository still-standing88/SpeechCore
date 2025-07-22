
#include "pc_talker.h"

ScreenReaderPCTalker::ScreenReaderPCTalker() :
    ScreenReader(L"PC Talker", SC_HAS_SPEECH | SC_HAS_SPEECH_STATE),
    module(NULL),
    pctk_status_fn(NULL),
    pctk_read_fn(NULL),
    pctk_reset_fn(NULL),
    loaded(false),
    Is_Active(false),
    IsSpeaking(false) {
}

ScreenReaderPCTalker::~ScreenReaderPCTalker() {
    this->release();
}

void ScreenReaderPCTalker::init() {
    if (!this->loaded) {
        this->module = LoadLibrary(PCTALKER_MODULE);
        if (this->module) {
            pctk_status_fn = (PCTKStatus_t)GetProcAddress(this->module, "PCTKStatus");
            pctk_read_fn = (PCTKPRead_t)GetProcAddress(this->module, "PCTKPRead");
            pctk_reset_fn = (PCTKVReset_t)GetProcAddress(this->module, "PCTKVReset");
            
            if (pctk_status_fn && pctk_read_fn && pctk_reset_fn) {
                this->loaded = true;
            }
        }
        
        if (this->loaded && !this->Is_Active) {
            this->Is_Active = this->is_running();
        }
    }
}

void ScreenReaderPCTalker::release() {
    if (this->module) {
        this->loaded = false;
        this->Is_Active = false;
        this->IsSpeaking = false;
        FreeLibrary(this->module);
        this->module = NULL;
    }
}

bool ScreenReaderPCTalker::is_running() {
    if (this->module && pctk_status_fn) {
        return pctk_status_fn() != 0;
    }
    return false;
}

bool ScreenReaderPCTalker::speak_text(const wchar_t* text, bool interrupt) {
    if (this->module && pctk_read_fn) {
        if (interrupt) {
            this->stop_speech();
        }
        
        std::string cp932_text = wstring_to_cp932(text);        
        int result = pctk_read_fn(cp932_text.c_str(), 0, 1);
               
        return result == 0;
    }
    return false;
}

bool ScreenReaderPCTalker::speak_text(const wchar_t* text, bool interrupt, bool with_ssml) {
return this->speak_text(text, interrupt);
}

bool ScreenReaderPCTalker::stop_speech() {
    if (this->module && pctk_reset_fn) {
        int result = pctk_reset_fn();
        return result == 0;
    }
    return false;
}

std::string ScreenReaderPCTalker::wstring_to_cp932(const wchar_t* wstr) {
    if (!wstr) return "";    
    int size = WideCharToMultiByte(932, 0, wstr, -1, NULL, 0, NULL, NULL);
    if (size <= 0) return "";
    
    std::string result(size, 0);
    WideCharToMultiByte(932, 0, wstr, -1, &result[0], size, NULL, NULL);

        if (!result.empty() && result.back() == '\0') {
        result.pop_back();
    }
    
    return result;
}