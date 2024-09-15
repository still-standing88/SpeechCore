#include "SpeechDispatcher.h"
#ifdef __linux__ || __unix__
#include <dlfcn.h>
#endif // __linux__ || __unix__

#include <cstring>
#include <cwchar>
#include <vector>

SpeechDispatcher::SpeechDispatcher()
    : ScreenReader(L"Speech Dispatcher", SC_HAS_SPEECH | SC_SPEECH_PARAMETER_CONTROL),
    speech_connection(nullptr), lib_handle(nullptr) {}

SpeechDispatcher::~SpeechDispatcher() {
    release();
}

void SpeechDispatcher::init() {
    lib_handle = dlopen("libspeechd.so", RTLD_LAZY);
    if (!lib_handle) {
        return;
    }

    load_function(spd_get_default_address, "spd_get_default_address");
    load_function(spd_open2, "spd_open2");
    load_function(spd_close, "spd_close");
    load_function(spd_say, "spd_say");
    load_function(spd_stop, "spd_stop");
    load_function(spd_set_volume, "spd_set_volume");
    load_function(spd_get_volume, "spd_get_volume");
    load_function(spd_set_voice_rate, "spd_set_voice_rate");
    load_function(spd_get_voice_rate, "spd_get_voice_rate");

    if (spd_get_default_address && spd_open2) {
        const auto* address = spd_get_default_address(nullptr);
        if (address) {
            speech_connection = spd_open2("SRAL", nullptr, nullptr, SPD_MODE_THREADED, address, true, nullptr);
        }
    }
}

void SpeechDispatcher::release() {
    if (speech_connection) {
        spd_close(speech_connection);
        speech_connection = nullptr;
    }
    if (lib_handle) {
        dlclose(lib_handle);
        lib_handle = nullptr;
    }
}

bool SpeechDispatcher::is_running() {
    return speech_connection != nullptr;
}

bool SpeechDispatcher::is_speaking() {
    // Speech Dispatcher doesn't provide a direct method to check if it's currently speaking
    // You might need to implement a custom solution if this functionality is crucial
    return false;
}

bool SpeechDispatcher::speak_text(const wchar_t* text, bool interrupt) {
    if (!speech_connection) return false;

    std::vector<char> utf8_text(wcslen(text) * 4 + 1);  // 4 bytes per character should be enough for UTF-8
    wcstombs(utf8_text.data(), text, utf8_text.size());

    if (interrupt) {
        stop_speech();
    }

    return spd_say(speech_connection, interrupt ? SPD_IMPORTANT : SPD_TEXT, utf8_text.data()) == 0;
}

bool SpeechDispatcher::stop_speech() {
    if (!speech_connection) return false;
    return spd_stop(speech_connection) == 0;
}

float SpeechDispatcher::get_volume() const {
    if (!speech_connection) return 0.0f;
    return static_cast<float>(spd_get_volume(speech_connection)) / 100.0f;
}

void SpeechDispatcher::set_volume(float offset) {
    if (!speech_connection) return;
    int volume = static_cast<int>(offset * 100.0f);
    spd_set_volume(speech_connection, volume);
}

float SpeechDispatcher::get_rate() const {
    if (!speech_connection) return 0.0f;
    return static_cast<float>(spd_get_voice_rate(speech_connection)) / 100.0f;
}

void SpeechDispatcher::set_rate(float offset) {
    if (!speech_connection) return;
    int rate = static_cast<int>(offset * 100.0f);
    spd_set_voice_rate(speech_connection, rate);
}

template<typename T>
void SpeechDispatcher::load_function(T& func_ptr, const char* func_name) {
    func_ptr = reinterpret_cast<T>(dlsym(lib_handle, func_name));
}
