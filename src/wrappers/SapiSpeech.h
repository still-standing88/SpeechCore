#pragma once
#pragma warning(disable: 4996) // Disable deprecated warning for GetVersionExW
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <minwindef.h>
#include <sapi.h>
#include <sphelper.h>
#include <sperror.h>
#include <atlbase.h>
#include <atlcom.h>
#include <thread>
#include <deque>
#include <mutex>
#include <condition_variable>
#include <string>
#include <vector>
#include "../ThirdParty/wasapi.h"

struct TtsMsg {
	const wchar_t* text;
	bool interrupt;
	bool xml;
};

class Sapi5Speech {
private:
	std::mutex msg_mutex;
	std::condition_variable msg_condition;
	bool processing;
	bool _is_speaking;
	std::deque<TtsMsg> messages;
	std::thread task_thread;
	std::thread task_thread2;
	WasapiPlayer* audio_playback;
	CComPtr<ISpVoice> voice;
	CComPtr<ISpStream> file_stream;
	CComPtr<ISpStream> memory_stream;
	CComPtr<IStream> output_stream;
	CSpStreamFormat audio_format;
	WAVEFORMATEX format;
	std::vector<BYTE> audio_data;
std::vector<CComPtr<ISpObjectToken>> voice_tokens;
	const wchar_t* get_voice_name(CComPtr<ISpObjectToken> _voice_token);

	void retreive_voice_tokens();
	void speak_memory(const wchar_t* _text, bool interrupt = false, bool xml = false);
	void set_format_data();
	void trimSilence(std::vector<BYTE> audio);
	void processMessages();
	void init();
	void free();

public:
	Sapi5Speech();
	~Sapi5Speech();
	long get_rate();
	void set_rate(long _rate);
	USHORT get_volume();
	void set_volume(USHORT _volume);
	const wchar_t* get_voice();
	void set_voice(std::wstring _voice_name);
	bool is_speaking();
	bool is_active();
	void speak_text(const wchar_t* _text,bool interrupt=false,bool xml=false);
	void speak_to_file(const char* filename, const wchar_t* _text , bool xml=false);
	const wchar_t* get_voice_by_index(int index);
	void set_voice_by_index(int index);
	int get_voices();
	void pause_speach();
	void resume_speach();
	void stop_speach();
};
