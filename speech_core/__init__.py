# SpeechCore python wrapper.
# functions have bin separated into 2 classes: 
# SpeechCore contains all of the speech methods to manage drivers.
# Sapi contains sapi only methods.
# Only one instance of SpeechCore mey exist at a time, this goes for sapi as well.
# All functions have bin converted to lower snake case and the Speech prefix have bin removed.

import os, sys

from .__speech_common import *
from .SpeechCore import (
    init, is_loaded, free, resume, pause, stop,
    output, output_file, braille,
    set_driver, get_driver, get_drivers, current_driver, detect_driver,
    get_voice, get_voices, set_voice,
    get_rate, set_rate, get_volume, set_volume,
    get_flags
    )

if sys.platform == "win32":
    from .__speech_sapi import Sapi
    
    module_path = os.path.dirname(os.path.abspath(__file__))


def add_dll_directory(dll_path):
    os.add_dll_directory(dll_path)
    os.environ["PATH"] += dll_path+os.pathsep


class SpeechCore:

    @classmethod
    def init(cls):
        try:
            init()
        except Exception as e:
            raise InitializationError(f'Failed initializing SpeechCore {str(e)}')

    @classmethod
    def free(cls):
        if cls.is_loaded():
            free()

    @classmethod
    def prefer_sapi(cls, prefer_sapi: bool):
        prefer_sapi(prefer_sapi)

    @classmethod
    def is_loaded(cls) -> bool:
        return is_loaded()

    def __enter__(self):
        self.init()
        return self

    def __exit__(self, exc_type, exc_value, traceback):
        self.free()
        return False

    @CheckInit
    def detect_driver(self) ->None :
        detect_driver()

    @CheckInit
    def get_driver(self, index: int) ->str :
        return get_driver(index)

    @CheckInit
    def current_driver(self) ->str :
        return current_driver()

    @CheckInit
    def set_driver(self, index: int) ->None :
        set_driver(index)

    @CheckInit
    def get_drivers(self) ->int :
        return get_drivers()

    @CheckInit
    def get_voice(self, index: int) ->str :
        return get_voice(index)

    @CheckInit
    def get_current_voice(self) ->str :
        return get_current_voice()

    @CheckInit
    def set_voice(self, index: int) ->None :
        set_voice(index)

    @CheckInit
    def get_voices(self) ->int :
        return get_voices()

    @CheckInit
    def set_volume(self, offset: float) ->None :
        set_volume(offset)

    @CheckInit
    def get_volume(self) ->float :
        return get_volume()

    @CheckInit
    def set_rate(self, offset: float) ->None :
        set_rate(offset)

    @CheckInit
    def get_rate(self) ->float :
        return get_rate()

    @CheckInit
    def is_speaking(self) ->bool :
        return is_speaking()


    @CheckInit
    def get_speech_flags(self) ->int :
        return get_flags()

    def check_speech_flags(self, flags: int) -> bool:
        return (self.get_speech_flags() & flags)

    @CheckInit
    def output(self, text: str, interrupt: bool = False) -> bool:
        return output(text, interrupt)

    @CheckInit
    def output_braille(self, text: str) ->bool :
        return braille(text)

    @CheckInit
    def output_file(self, filename: str, text: str) ->None :
        output_file(file_name, text)

    @CheckInit
    def resume(self) ->None :
        resume()

    @CheckInit
    def pause(self) ->None :
        pause()

    @CheckInit
    def stop(self) ->None :
        stop()


__all__ = [
        "init", "free", "resume", "pause", "stop",
    "output", "output_file", "braille",
    "set_driver", "get_driver", "get_drivers", "current_driver", "detect_driver",
    "get_voice", "get_voices", "set_voice",
    "get_rate", "set_rate", "get_volume", "set_volume",
    "get_flags",
    "SpeechCore", "Sapi"
    ]

if sys.platform == "win32":
    module_path = os.path.dirname(os.path.abspath(__file__))
    add_dll_directory(module_path)
