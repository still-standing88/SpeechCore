#ifndef SA_API_H
#define SA_API_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <wchar.h>

// Initialize the SA API
bool sa_initialize();

// Clean up and release resources
void sa_cleanup();

// Speak text
bool sa_speak(const wchar_t *text, bool interrupt);

// Display text on braille device
bool sa_braille(const wchar_t *text);

// Stop all audio output
bool sa_silence();

// Check if System Access is running
bool sa_is_active();

// Output text (speak and braille)
bool sa_output(const wchar_t *text, bool interrupt);

#ifdef __cplusplus
}
#endif

#endif // SA_API_H