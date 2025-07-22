import sys
from functools import wraps

from .SpeechCore import (
    SC_SPEECH_FLOW_CONTROL, SC_SPEECH_PARAMETER_CONTROL, SC_VOICE_CONFIG, SC_FILE_OUTPUT,
    SC_HAS_SPEECH, SC_HAS_BRAILLE, SC_HAS_SPEECH_STATE
    )

class InitializationError(Exception):
    pass

class NotLoadedError(Exception):
    pass

def CheckInit(func):
    @wraps(func)
    def wrapper(self, *args, **kw):
        if self.is_loaded():
            res = func(self, *args, **kw)
            return res
        else:
            raise NotLoadedError("speech_core is not loaded. Initialize SpeechCore before calling any method.")
    return wrapper

def CheckSapi(func):
    @wraps(func)
    def wrapper(self, *args, **kw):
        if sys.platform == 'win32':
            if not self.sapi_loaded():
                raise NotLoadedError('Sapi is not loaded. Initialize sapi before calling any function.')
            res = func(self, *args, **kw)
            return res
        else:
            raise NotImplementedError(f'Sapi functions are not available on {sys.platform}.')
    return wrapper

__all__ = [
    "SC_SPEECH_FLOW_CONTROL", "SC_SPEECH_PARAMETER_CONTROL", "SC_VOICE_CONFIG", "SC_FILE_OUTPUT",
    "SC_HAS_SPEECH", "SC_HAS_BRAILLE", "SC_HAS_SPEECH_STATE",
    "InitializationError", "NotLoadedError", "CheckInit", "CheckSapi"
    ]