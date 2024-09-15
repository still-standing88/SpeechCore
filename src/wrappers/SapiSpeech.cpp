#include "SapiSpeech.h"
#include <stdexcept>
#include <conio.h>


Sapi5Speech::Sapi5Speech(): processing(false) {
	init();


	this->processing = true;
	WasapiPlayer::ChunkCompletedCallback cb = nullptr;
	wchar_t device[] = L"";
	CreateStreamOnHGlobal(NULL,true, &this->output_stream);
	this->audio_format.AssignFormat(SPSF_22kHz16BitMono);
	this->set_format_data();
	this->audio_playback = new WasapiPlayer(device, this->format, cb);
	this->audio_playback->open();
	this->task_thread = std::thread([&]() {processMessages(); });
	this->retreive_voice_tokens();
}
Sapi5Speech::~Sapi5Speech() {
	free();
	}
void Sapi5Speech::init() {
	auto result = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	if (FAILED(result)) {
		CoUninitialize();
		//throw std::runtime_error("Failed to Initialize com");
	}

	auto res = CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_INPROC_SERVER, IID_ISpVoice, (void**)&this->voice);
	if (!SUCCEEDED(res)) {
		throw std::runtime_error("Failed to create sapi5 voice instance");
}
}
void Sapi5Speech::free() {    
    if (audio_playback) {
        audio_playback->stop();
    }    
    processing = false;
 msg_condition.notify_all();
    if (task_thread.joinable()) {
        task_thread.join();
    }    
    if (voice) {
        voice.Release();
    }
    delete audio_playback;    
    file_stream.Release();
    memory_stream.Release();
    output_stream.Release();    
    CoUninitialize();    
}
void Sapi5Speech::set_format_data() {
	this->format.wFormatTag = WAVE_FORMAT_PCM;
	this->format.cbSize = 0;
	this->format.nSamplesPerSec = 16000;
	this->format.wBitsPerSample = 16;
	this->format.nChannels = 1;
	this->format.nBlockAlign = (this->format.nChannels * this->format.wBitsPerSample) / 8;
	this->format.nAvgBytesPerSec = this->format.nSamplesPerSec * this->format.nBlockAlign;
}

void Sapi5Speech::trimSilence(std::vector<BYTE> audio) {
	std::vector<float> audioFloat(audio.size() / 2);
	for (size_t i = 0; i < audioFloat.size(); ++i) {
		int16_t sample = static_cast<int16_t>(audio[i * 2] | (audio[i * 2 + 1] << 8));
		audioFloat[i] = static_cast<float>(sample) / 32768.0f;
	}

	int start = 0;
	while (start < static_cast<int>(audioFloat.size()) && std::abs(audioFloat[start]) <= 0.01f) {
		start++;
	}

	int end = static_cast<int>(audioFloat.size()) - 1;
	while (end >= 0 && std::abs(audioFloat[end]) <= 0.01f) {
		end--;
	}

	audio.erase(audio.begin(), audio.begin() + start * 2);
	audio.erase(audio.begin() + (end - start + 1) * 2, audio.end());
}

const wchar_t* Sapi5Speech::get_voice_name(CComPtr<ISpObjectToken> _voice_token) {
	if (!_voice_token) {return L"";}
	WCHAR* voice_id;
	_voice_token->GetId(&voice_id);
	CSpDynamicString SPVDStr;
	auto res = SpGetDescription(_voice_token, &SPVDStr);
	if (SUCCEEDED(res)) {
		BSTR SPVDBstr;
		SPVDStr.CopyToBSTR(&SPVDBstr);
		wchar_t* voice_name = _wcsdup(SPVDStr);
		SysFreeString(SPVDBstr);
		return voice_name;
	}
	return L"";
}
void Sapi5Speech::retreive_voice_tokens() {
	std::thread([&]() {
		IEnumSpObjectTokens* token_enums;
		unsigned long token_count;
		SpEnumTokens(SPCAT_VOICES, NULL, NULL, &token_enums);
		token_enums->GetCount(&token_count);
		this->voice_tokens.clear();
		this->voice_tokens.shrink_to_fit();
		for (unsigned long i = 0; i < token_count; i++) {
			CComPtr<ISpObjectToken> voice_token;
			token_enums->Item(i, &voice_token);
			if (this !=nullptr) {this->voice_tokens.push_back(voice_token); }
		}
	
		}).detach();
}

void Sapi5Speech::processMessages() {
	while (this->processing) {
		std::unique_lock<std::mutex> lock(msg_mutex);
		msg_condition.wait(lock, [&]() {return !messages.empty() || !this->processing; });
if (!this->processing) {break;}
			this->_is_speaking = true;
			unsigned int id;
			TtsMsg message = this->messages.front();
			this->messages.pop_front();
			this->speak_memory(message.text, message.interrupt, message.xml);
			trimSilence(audio_data);
			audio_playback->feed(audio_data.data(), static_cast<int> (audio_data.size()), &id);
			audio_playback->sync();
			this->_is_speaking = false;

	}
}

long Sapi5Speech::get_rate() {
	long speed;
	this->voice->GetRate(&speed);
	return speed;
}
void Sapi5Speech::set_rate(long _rate) {
	this->voice->SetRate(_rate);
	}
USHORT Sapi5Speech::get_volume() {
	USHORT volume;
	this->voice->GetVolume(&volume);
	return volume;
	}
void Sapi5Speech::set_volume(USHORT _volume) {
	this->voice->SetVolume(_volume);
	}
const wchar_t* Sapi5Speech::get_voice() {
	CComPtr<ISpObjectToken> voice_token;
	auto res = this->voice->GetVoice(&voice_token);
	if (SUCCEEDED(res)) {
		return this->get_voice_name(voice_token);
	}
	return L"";
	}
void Sapi5Speech::set_voice(std::wstring _voice_name) {

	for (auto element : this->voice_tokens) {
		std::wstring voice_name = this->get_voice_name(element);
		if (_voice_name == voice_name) {
			this->voice->SetVoice(element);
			break;
		}
	}

	}

void Sapi5Speech::set_voice_by_index(int index) {
	if (index < static_cast<int> (this->voice_tokens.size())) {
		this->voice->SetVoice(this->voice_tokens[index]);
}
}

const wchar_t* Sapi5Speech::get_voice_by_index(int index) {
	if (index < static_cast<int> (this->voice_tokens.size())) {
		return this->get_voice_name(this->voice_tokens[index]);
	}
	return NULL;
}

int Sapi5Speech::get_voices() {
	if (this->voice_tokens.empty()) {
		this->retreive_voice_tokens();
	}
	return static_cast<int> (this->voice_tokens.size());
}
bool Sapi5Speech::is_speaking() {
	return this->_is_speaking;
}
bool Sapi5Speech::is_active() {
	if (this->voice) {
		return true;
}
	return false;
}

void Sapi5Speech::speak_text(const wchar_t* _text,bool interrupt, bool xml) {
	if (this->voice) {
		if (interrupt) {
			this->audio_playback->stop();
			this->messages.clear();
			this->_is_speaking = false;
		}
		std::lock_guard<std::mutex> lock(msg_mutex);
		messages.emplace_back(TtsMsg(_text, interrupt, xml));
		msg_condition.notify_one();
	}
	else {
		throw std::runtime_error("Error voice not initialized");
	}
	}
void Sapi5Speech::speak_memory(const wchar_t* _text, bool interrupt , bool xml ) {
	if (!this->voice) {
		throw std::runtime_error("Error voice not initialized");
	}
	DWORD flags = SVSFlagsAsync;
	xml == true ? flags |= SPF_IS_XML : flags |= SPF_IS_NOT_XML;
	interrupt == true ? flags |= SVSFPurgeBeforeSpeak : flags;
	if (this->memory_stream) { this->memory_stream.Release(); }
	if (this->output_stream) { this->output_stream.Release(); }
	auto res = CreateStreamOnHGlobal(NULL, true, &this->output_stream);
	res = CoCreateInstance(CLSID_SpStream, NULL, CLSCTX_ALL, IID_ISpStream, (void**)&this->memory_stream);
	if (SUCCEEDED(res)) {
		res = this->memory_stream->SetBaseStream(output_stream, SPDFID_WaveFormatEx, &this->format);
		if (SUCCEEDED(res)) {
			this->voice->SetOutput(this->memory_stream, true);
			this->voice->Speak(_text,flags,NULL);
			this->voice->WaitUntilDone(-1);
			STATSTG stats;
			LARGE_INTEGER large_int = { 0 };
			ULONG audio_bytes;
			this->output_stream->Stat(&stats, STATFLAG_NONAME);
			this->audio_data.resize(stats.cbSize.LowPart);
			this->output_stream->Seek(large_int,STREAM_SEEK_SET,NULL);
			res = this->output_stream->Read(this->audio_data.data(), stats.cbSize.LowPart, &audio_bytes);
}
	}
}

void Sapi5Speech::speak_to_file(const char* filename, const wchar_t* _text, bool xml) {
	if (!this->voice) {
		throw std::runtime_error("Error voice not initialized");
	}
	DWORD flags = SVSFlagsAsync;
	xml == true ? flags |= SPF_IS_XML : flags |= SPF_IS_NOT_XML;
	if (this->file_stream) {
		this->file_stream->Close();
	}
	auto res = SPBindToFile(filename, SPFM_CREATE_ALWAYS, &this->file_stream, &this->audio_format.FormatId(), this->audio_format.WaveFormatExPtr());
	if (SUCCEEDED(res)) {
		this->voice->SetOutput(this->file_stream, true);
		this->voice->Speak(_text, flags, NULL);
	}
}

void Sapi5Speech::pause_speach() {
	this->audio_playback->pause();
}
void Sapi5Speech::resume_speach() {
	this->audio_playback->resume();
}
void Sapi5Speech::stop_speach() {
	this->audio_playback->stop();
}
