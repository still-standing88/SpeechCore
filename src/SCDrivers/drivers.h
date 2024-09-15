#pragma once
#ifdef _WIN32
#include "jaws.h"
#include "nvda.h"
#include "sa.h"
#include "sapi5driver.h"
#include "zdsr.h"
#elif defined(__APPLE__)
#include "AVTts.h"
#elif defined(__linux__) || defined(__unix__)
#include "SpeechDispatcher.h"
#else
#error "Unknown Platform"
#endif // _WIN32

