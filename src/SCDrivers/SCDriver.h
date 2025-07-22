// ScreenReader abstract class. Override this class to implement new screen readers.
#pragma once
#include "../../include/SpeechCore.h"

class ScreenReader {
protected:
	const wchar_t* screen_reader_name;
	uint32_t speech_flags; // These flags indicate the abilities of the screen reader.

public:
	ScreenReader(const wchar_t* name, uint32_t flags = 0):
		screen_reader_name(name), speech_flags(flags) {}
	virtual ~ScreenReader() {}

// Initialization and freeing methods. Can be called on constructor and destructor. Although it's genrally preferable to control it manually.
	virtual void init() = 0;
	virtual void release() = 0;

// Not all screen readers support this method or offer a way to know if the screen reader is running. 
// Default is just to check for the presence of the screen reader binaries if any.
	virtual bool is_running() = 0;
// Same as the method above. Not all screen readers support it.
	virtual bool is_speaking() = 0;

	virtual bool speak_text(const wchar_t* text,bool interrupt=false) =0;
	virtual bool speak_text(const wchar_t* text,bool interrupt=false, bool with_ssml = False) {}
	virtual bool stop_speech() =0;
	virtual bool output_braille(const wchar_t* text) { return false; }
	virtual void output_file(const char* filePath, const wchar_t* text) {}

// The following methods are genrally not supported by screen readers, only system native.
	virtual void resume_speech() {}
	virtual void pause_speech() {}

// Voice parameters
	virtual float get_volume() const { return 0; }
	virtual void set_volume(float offset) {}
	virtual float get_rate() const { return 0; }
	virtual void set_rate(float offset) {}
	virtual float get_pitch() const { return 0; }
	virtual void set_pitch(float offset) {}

// Voice control methods
	virtual const wchar_t* get_voice(int index) const { return 0; }
	virtual void set_voice(int index) {}
	virtual const wchar_t* get_current_voice() const { return L""; }
	virtual int get_voices() const { return 0; }

// Default methods for retrieving screen reader info.
	const wchar_t* get_name() const { return this->screen_reader_name; }
	uint32_t get_speech_flags() const { return this->speech_flags; }
};