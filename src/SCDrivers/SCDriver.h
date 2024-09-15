#pragma once
#include "../../include/SpeechCore.h"

class ScreenReader {
protected:
	const wchar_t* screen_reader_name;
	uint32_t speech_flags;

public:
	ScreenReader(const wchar_t* name, uint32_t flags = 0):
		screen_reader_name(name), speech_flags(flags) {}
	virtual ~ScreenReader() {}
	virtual void init() = 0;
	virtual void release() = 0;
	virtual bool is_running() = 0;
	virtual bool is_speaking() = 0;
	virtual bool speak_text(const wchar_t* text,bool interrupt=false) =0;
	virtual bool stop_speech() =0;
	virtual bool output_braille(const wchar_t* text) { return false; }
	virtual void output_file(const char* filePath, const wchar_t* text) {}
	virtual void resume_speech() {}
	virtual void pause_speech() {}
	virtual float get_volume() const { return 0; }
	virtual void set_volume(float offset) {}
	virtual float get_rate() const { return 0; }
	virtual const wchar_t* get_voice(int index) const { return 0; }
	virtual void set_voice(int index) {}
	virtual const wchar_t* get_current_voice() const { return L""; }
	virtual int get_voices() const { return 0; }
	virtual void set_rate(float offset) {}
	const wchar_t* get_name() const { return this->screen_reader_name; }
	uint32_t get_speech_flags() const { return this->speech_flags; }
};