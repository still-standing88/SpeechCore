// Thanks John C for providing the skeleton code from which i've built this very wrapper.
#import <AVFoundation/AVFoundation.h>
#import <Foundation/Foundation.h>
#include "AVSpeech.h"
#include <vector>

class AVTTSVoice::Impl {
public:
    float rate;
    float volume;
    float pitch;
    AVSpeechSynthesizer* synth;
    AVSpeechSynthesisVoice* currentVoice;
    AVSpeechUtterance* utterance;

    Impl() {
        currentVoice = [AVSpeechSynthesisVoice voiceWithLanguage:@"en-US"];
        utterance = [[AVSpeechUtterance alloc] initWithString:@""];
        rate = utterance.rate;
        volume = utterance.volume;
        pitch = utterance.pitchMultiplier;
        synth = [[AVSpeechSynthesizer alloc] init];
    }

    Impl(const std::string& language) {
        NSString* nsLanguage = [NSString stringWithUTF8String:language.c_str()];
        currentVoice = [AVSpeechSynthesisVoice voiceWithLanguage:nsLanguage];
        if (!currentVoice) currentVoice = [AVSpeechSynthesisVoice voiceWithLanguage:@"en-US"];
        utterance = [[AVSpeechUtterance alloc] init];
        rate = utterance.rate;
        volume = utterance.volume;
        pitch = utterance.pitchMultiplier;
        synth = [[AVSpeechSynthesizer alloc] init];
    }

    bool speak(const std::string& text, bool interrupt) {
        if (interrupt && synth.isSpeaking) [synth stopSpeakingAtBoundary:AVSpeechBoundaryImmediate];
        if (text.empty()) return interrupt;
        NSString* nsText = [NSString stringWithUTF8String:text.c_str()];
        utterance = [[AVSpeechUtterance alloc] initWithString:nsText];
        utterance.rate = rate;
        utterance.volume = volume;
        utterance.pitchMultiplier = pitch;
        utterance.voice = currentVoice;
        [synth speakUtterance:utterance];
        return synth.isSpeaking;
    }

    bool speakWait(const std::string& text, bool interrupt) {
        bool result = speak(text, interrupt);
        while (synth.isSpeaking) {
            [NSThread sleepForTimeInterval:0.05];
        }
        return result;
    }

    bool stopSpeech() {
        if (synth.isSpeaking) return [synth stopSpeakingAtBoundary:AVSpeechBoundaryImmediate];
        return false;
    }


    bool pauseSpeech() {
        if (!synth.isPaused && synth.isSpeaking) {
            return [synth pauseSpeakingAtBoundary:AVSpeechBoundaryImmediate];
        }
        return false;
    }

    bool resumeSpeech() {
        if (synth.isPaused) {
            return [synth continueSpeaking];
        }
        return false;
    }

    bool isPaused() const { return synth.isPaused; }
    bool isSpeaking() const { return synth.isSpeaking; }


    std::string getCurrentVoice() const {
        return [currentVoice.name UTF8String];
    }

    std::vector<std::string> getAllVoices() const {
        NSArray<AVSpeechSynthesisVoice*>* voices = [AVSpeechSynthesisVoice speechVoices];
        std::vector<std::string> voiceNames;
        for (AVSpeechSynthesisVoice* voice in voices) {
            voiceNames.push_back([voice.name UTF8String]);
        }
        return voiceNames;
    }

    std::vector<std::string> getVoicesByLanguage(const std::string& language) const {
        NSArray<AVSpeechSynthesisVoice*>* voices = [AVSpeechSynthesisVoice speechVoices];
        NSString* nsLanguage = [NSString stringWithUTF8String:language.c_str()];
        std::vector<std::string> voiceNames;
        for (AVSpeechSynthesisVoice* voice in voices) {
            if ([voice.language isEqualToString:nsLanguage]) {
                voiceNames.push_back([voice.name UTF8String]);
            }
        }
        return voiceNames;
    }

    void setVoiceByName(const std::string& name) {
        NSString* nsName = [NSString stringWithUTF8String:name.c_str()];
        NSArray<AVSpeechSynthesisVoice*>* voices = [AVSpeechSynthesisVoice speechVoices];
        for (AVSpeechSynthesisVoice* voice in voices) {
            if ([voice.name isEqualToString:nsName]) {
                currentVoice = voice;
                break;
            }
        }
    }

    void setVoiceByLanguage(const std::string& language) {
        NSString* nsLanguage = [NSString stringWithUTF8String:language.c_str()];
        NSArray<AVSpeechSynthesisVoice*>* voices = [AVSpeechSynthesisVoice speechVoices];
        for (AVSpeechSynthesisVoice* voice in voices) {
            if ([voice.language isEqualToString:nsLanguage]) {
                currentVoice = voice;
                break;
            }
        }
    }

    std::string getCurrentLanguage() const {
        return [currentVoice.language UTF8String];
    }

    size_t getVoicesCount() const {
        return [[AVSpeechSynthesisVoice speechVoices] count];
    }

    int getVoiceIndex(const std::string& name) const {
        NSArray<AVSpeechSynthesisVoice*>* voices = [AVSpeechSynthesisVoice speechVoices];
        NSString* nsName = [NSString stringWithUTF8String:name.c_str()];
        for (NSUInteger i = 0; i < voices.count; ++i) {
            if ([voices[i].name isEqualToString:nsName]) {
                return static_cast<int>(i);
            }
        }
        return -1;
    }

    bool setVoiceByIndex(size_t index) {
        NSArray<AVSpeechSynthesisVoice*>* voices = [AVSpeechSynthesisVoice speechVoices];
        if (index < voices.count) {
            currentVoice = voices[index];
            return true;
        }
        return false;
    }

    std::string getVoiceName(size_t index) {
        NSArray<AVSpeechSynthesisVoice*>* voices = [AVSpeechSynthesisVoice speechVoices];
        if (index < voices.count) {
            return [voices[index].name UTF8String];
        }
        return "";
    }
};

AVTTSVoice::AVTTSVoice() : pImpl(new Impl()) {}
AVTTSVoice::AVTTSVoice(const std::string& language) : pImpl(new Impl(language)) {}
AVTTSVoice::~AVTTSVoice() { delete pImpl; }

bool AVTTSVoice::speak(const std::string& text, bool interrupt) { return pImpl->speak(text, interrupt); }
bool AVTTSVoice::speakWait(const std::string& text, bool interrupt) { return pImpl->speakWait(text, interrupt); }
bool AVTTSVoice::stopSpeech() { return pImpl->stopSpeech(); }
bool AVTTSVoice::pauseSpeech() { return pImpl->pauseSpeech(); }
bool AVTTSVoice::resumeSpeech() { return pImpl->resumeSpeech(); }
bool AVTTSVoice::isPaused() const { return pImpl->isPaused(); }
bool AVTTSVoice::isSpeaking() const { return pImpl->isSpeaking(); }

std::string AVTTSVoice::getCurrentVoice() const { return pImpl->getCurrentVoice(); }
std::vector<std::string> AVTTSVoice::getAllVoices() const { return pImpl->getAllVoices(); }
std::vector<std::string> AVTTSVoice::getVoicesByLanguage(const std::string& language) const { return pImpl->getVoicesByLanguage(language); }

void AVTTSVoice::setVoiceByName(const std::string& name) { pImpl->setVoiceByName(name); }
void AVTTSVoice::setVoiceByLanguage(const std::string& language) { pImpl->setVoiceByLanguage(language); }
std::string AVTTSVoice::getCurrentLanguage() const { return pImpl->getCurrentLanguage(); }
size_t AVTTSVoice::getVoicesCount() const { return pImpl->getVoicesCount(); }
int AVTTSVoice::getVoiceIndex(const std::string& name) const { return pImpl->getVoiceIndex(name); }
bool AVTTSVoice::setVoiceByIndex(size_t index) { return pImpl->setVoiceByIndex(index); }
std::string AVTTSVoice::getVoiceName(size_t index) { return pImpl->getVoiceName(index); }

void AVTTSVoice::setRate(float rate) { pImpl->rate = rate; }
float AVTTSVoice::getRate() const { return pImpl->rate; }
void AVTTSVoice::setVolume(float volume) { pImpl->volume = volume; }
float AVTTSVoice::getVolume() const { return pImpl->volume; }
void AVTTSVoice::setPitch(float pitch) { pImpl->pitch = pitch; }
float AVTTSVoice::getPitch() const { return pImpl->pitch; }