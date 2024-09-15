#pragma once
#include "SCDriver.h"
#include "../wrappers/SapiSpeech.h"

class ScreenReaderSapi5 : public ScreenReader {
private:
	Sapi5Speech *module;
	bool loaded;
	bool Is_Active;

public:
	ScreenReaderSapi5(Sapi5Speech *sapi_instance);
	~ScreenReaderSapi5();
	void init() override;
	void release() override;
	bool is_speaking() override;
	bool is_running() override;
	bool speak_text(const wchar_t* text, bool interrupt = false) override;
	bool stop_speech() override;
	void output_file(const char* filePath, const wchar_t* text) override;
	float get_volume() const override;
	void set_volume(float offset) override;
float get_rate() const override;
	void set_rate(float offset) override;
	const wchar_t* get_voice(int index) const override;
	const wchar_t* get_current_voice() const override;
	void set_voice(int index) override;
	int get_voices() const override;
	void resume_speech() override;
	void pause_speech() override;
};