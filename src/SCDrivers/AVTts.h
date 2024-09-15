#pragma once
#include "SCDriver.h"
#include "../wrappers/AVSpeech.h"
#include <string>
#include <vector>

class AVTTSVoiceDriver : public ScreenReader {
public:
    AVTTSVoiceDriver();
    virtual ~AVTTSVoiceDriver();

    void init() override;
    void release() override;
    bool is_running() override;
    bool is_speaking() override;
    bool speak_text(const wchar_t* text, bool interrupt = false) override;
    bool stop_speech() override;
    void resume_speech() override;
    void pause_speech() override;
    float get_volume() const override;
    void set_volume(float offset) override;
    float get_rate() const override;
    void set_rate(float offset) override;
    const wchar_t* get_voice(int index) const override;
    void set_voice(int index) override;
    const wchar_t* get_current_voice() const override;
    int get_voices() const override;

private:
    AVTTSVoice* m_tts;
    std::vector<std::wstring> m_voices;
    std::wstring m_currentVoice;
};