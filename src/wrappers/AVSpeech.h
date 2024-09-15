#pragma once
#include <string>
#include <vector>

class AVTTSVoice {
public:
    AVTTSVoice();
    AVTTSVoice(const std::string& language);
    ~AVTTSVoice();

    bool speak(const std::string& text, bool interrupt = true);
    bool speakWait(const std::string& text, bool interrupt = true);
    bool stopSpeech();
    bool pauseSpeech();
    bool resumeSpeech();
    bool isPaused() const;
    bool isSpeaking() const;

    std::string getCurrentVoice() const;
    std::vector<std::string> getAllVoices() const;
    std::vector<std::string> getVoicesByLanguage(const std::string& language) const;

    void setVoiceByName(const std::string& name);
    void setVoiceByLanguage(const std::string& language);
    std::string getCurrentLanguage() const;
    size_t getVoicesCount() const;
    int getVoiceIndex(const std::string& name) const;
    bool setVoiceByIndex(size_t index);
    std::string getVoiceName(size_t index);

    void setRate(float rate);
    float getRate() const;
    void setVolume(float volume);
    float getVolume() const;
    void setPitch(float pitch);
    float getPitch() const;

private:
    class Impl;
    Impl* pImpl;
};