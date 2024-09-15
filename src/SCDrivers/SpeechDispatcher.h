#pragma once
#include "SCDriver.h"
#include <speech-dispatcher/libspeechd.h>

class SpeechDispatcher : public ScreenReader {
public:
    SpeechDispatcher();
    virtual ~SpeechDispatcher();

    void init() override;
    void release() override;
    bool is_running() override;
    bool is_speaking() override;
    bool speak_text(const wchar_t* text, bool interrupt = false) override;
    bool stop_speech() override;
    float get_volume() const override;
    void set_volume(float offset) override;
    float get_rate() const override;
    void set_rate(float offset) override;

private:
    SPDConnection* speech_connection;
    void* lib_handle;

    // Function pointers for dynamically loaded library functions
    SPDConnectionAddress* (*spd_get_default_address)(char**);
    SPDConnection* (*spd_open2)(const char*, const char*, const char*, SPDConnectionMode, const SPDConnectionAddress*, int, char**);
    void (*spd_close)(SPDConnection*);
    int (*spd_say)(SPDConnection*, SPDPriority, const char*);
    int (*spd_stop)(SPDConnection*);
    int (*spd_set_volume)(SPDConnection*, signed int);
    int (*spd_get_volume)(SPDConnection*);
    int (*spd_set_voice_rate)(SPDConnection*, signed int);
    int (*spd_get_voice_rate)(SPDConnection*);

    // Helper function to load library functions
    template<typename T>
    void load_function(T& func_ptr, const char* func_name);
};

