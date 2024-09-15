#include "AVTts.h"
#include <algorithm>
#include <codecvt>
#include <locale>
#include <string>
#include <vector>

namespace utils {
    std::wstring convertToWide(const std::string& str) {
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
        return converter.from_bytes(str);
    }

    std::string convertToUTF8(const wchar_t* text) {
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
        return converter.to_bytes(text);
    }
}

AVTTSVoiceDriver::AVTTSVoiceDriver()
    : ScreenReader(L"AVTTSVoice", SC_SPEECH_FLOW_CONTROL | SC_SPEECH_PARAMETER_CONTROL | SC_VOICE_CONFIG | SC_HAS_SPEECH),
      m_tts(nullptr) {
}

AVTTSVoiceDriver::~AVTTSVoiceDriver() {
    release();
}

void AVTTSVoiceDriver::init() {
    if (!m_tts) {
        m_tts = new AVTTSVoice();
        std::vector<std::string> voices = m_tts->getAllVoices();
        m_voices.clear();
        for (const auto& voice : voices) {
            m_voices.emplace_back(utils::convertToWide(voice));
        }
        if (!m_voices.empty()) {
            m_currentVoice = m_voices[0];
        }
    }
}

void AVTTSVoiceDriver::release() {
    if (m_tts) {
        delete m_tts;
        m_tts = nullptr;
    }
    m_voices.clear();
    m_currentVoice.clear();
}

bool AVTTSVoiceDriver::is_running() {
    return m_tts != nullptr;
}

bool AVTTSVoiceDriver::is_speaking() {
    return m_tts && m_tts->isSpeaking();
}

bool AVTTSVoiceDriver::speak_text(const wchar_t* text, bool interrupt) {
    if (m_tts) {
        std::string utf8Text = utils::convertToUTF8(text);
        return m_tts->speak(utf8Text, interrupt);
    }
    return false;
}

bool AVTTSVoiceDriver::stop_speech() {
    return m_tts && m_tts->stopSpeech();
}

void AVTTSVoiceDriver::resume_speech() {
    if (m_tts) {
        m_tts->resumeSpeech();
    }
}

void AVTTSVoiceDriver::pause_speech() {
    if (m_tts) {
        m_tts->pauseSpeech();
    }
}

float AVTTSVoiceDriver::get_volume() const {
    return m_tts ? m_tts->getVolume() : 0.0f;
}

void AVTTSVoiceDriver::set_volume(float offset) {
    if (m_tts) {
        float newVolume = std::max(0.0f, std::min(1.0f, get_volume() + offset));
        m_tts->setVolume(newVolume);
    }
}

float AVTTSVoiceDriver::get_rate() const {
    return m_tts ? m_tts->getRate() : 0.0f;
}

void AVTTSVoiceDriver::set_rate(float offset) {
    if (m_tts) {
        float newRate = std::max(0.0f, std::min(1.0f, get_rate() + offset));
        m_tts->setRate(newRate);
    }
}

const wchar_t* AVTTSVoiceDriver::get_voice(int index) const {
    if (index >= 0 && index < static_cast<int>(m_voices.size())) {
        return m_voices[index].c_str();
    }
    return nullptr;
}

void AVTTSVoiceDriver::set_voice(int index) {
    if (m_tts && index >= 0 && index < static_cast<int>(m_voices.size())) {
        std::string voiceName = utils::convertToUTF8(m_voices[index].c_str());
        m_tts->setVoiceByName(voiceName);
        m_currentVoice = m_voices[index];
    }
}

const wchar_t* AVTTSVoiceDriver::get_current_voice() const {
    return m_currentVoice.c_str();
}

int AVTTSVoiceDriver::get_voices() const {
    return static_cast<int>(m_voices.size());
}