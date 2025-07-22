import sys
from .__speech_common import *
from .SpeechCore import (
    sapi_init, sapi_loaded, sapi_release,
    sapi_resume, sapi_pause, sapi_stop,
    sapi_speak, sapi_output_file,
    sapi_voice_set_volume, sapi_voice_get_volume, sapi_voice_get_rate, sapi_voice_set_rate,
    sapi_get_current_voice, sapi_get_voices, sapi_set_voice, sapi_set_voice_by_index, sapi_get_voice
    )

class Sapi:

    @classmethod
    def init(cls):
        if sys.platform != 'win32':
            raise NotImplementedError(f'Sapi functions are not available on{sys.platform}')
        try:
            sapi_init()
        except Exception as e:
            raise InitializationError(f'Failed initializing sapi {str(e)}')

    @classmethod
    @CheckSapi
    def release(cls):
        sapi_release()

    @classmethod
    def sapi_loaded(cls)-> bool :
        return sapi_loaded()

    def __enter__(self):
        self.init()
        return self

    def __exit__(self, exc_type, exc_value, traceback):
        self.release()
        return False

    @CheckSapi
    def voice_set_rate(self, offset: float)->None :
        sapi_voice_set_rate(offset)

    @CheckSapi
    def voice_get_rate(self)->float :
        return sapi_voice_get_rate()

    @CheckSapi
    def voice_set_volume(self, offset: float)->None :
        sapi_voice_set_volume(offset)

    @CheckSapi
    def voice_get_volume(self)->float :
        return sapi_Voice_Get_Volume()

    @CheckSapi
    def get_voice(self, index: int) ->str :
        return sapi_get_voice(index)

    @CheckSapi
    def get_current_voice(self) ->str :
        return sapi_get_current_voice()

    @CheckSapi
    def set_voice_by_index(self, index: int) ->None :
        sapi_set_voice_by_index(index)

    @CheckSapi
    def set_voice(self, voice_name: str)->None :
        sapi_set_voice(voice_name)

    @CheckSapi
    def get_voices(self) ->int :
        return sapi_get_voices()

    @CheckSapi
    def speak(self, text: str, interrupt: bool = False, xml: bool = False) ->None :
            sapi_speak(text, interrupt, xml)

    @CheckSapi
    def output_file(self, filename: str, text: str, xml: bool = False) ->None :
        sapi_output_file(file_name, text, xml)

    @CheckSapi
    def resume(self)->None :
        sapi_resume()

    @CheckSapi
    def pause(self)->None :
        sapi_pause()

    @CheckSapi
    def stop(self)->None :
        sapi_stop()

__all__ = [
    "sapi_init", "sapi_loaded", "sapi_release",
    "sapi_resume", "sapi_pause", "sapi_stop",
    "sapi_speak", "sapi_output_file",
    "sapi_voice_set_volume", "sapi_voice_get_volume", "sapi_voice_get_rate", "sapi_voice_set_rate",
    "sapi_get_current_voice", "sapi_get_voices", "sapi_set_voice", "sapi_set_voice_by_index", "sapi_get_voice",
    "Sapi"
]
