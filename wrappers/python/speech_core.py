import ctypes
import sys
import os

class SpeechCore:
    SC_SPEECH_FLOW_CONTROL = 1 << 0
    SC_SPEECH_PARAMETER_CONTROL = 1 << 1
    SC_VOICE_CONFIG = 1 << 2
    SC_FILE_OUTPUT = 1 << 3
    SC_HAS_SPEECH = 1 << 4
    SC_HAS_BRAILLE = 1 << 5
    SAPI_ERROR = NotImplementedError("SAPI functions are not available on this platform")

    def __init__(self):
        self._has_sapi = False
        if sys.platform.startswith('win'):
            self.lib = ctypes.CDLL('./SpeechCore.dll')
            self._setup_sapi_function_prototypes()
            self._has_sapi = True
        elif sys.platform.startswith('linux'):
            self.lib = ctypes.CDLL('./SpeechCore.so')
        elif sys.platform.startswith('darwin'):
            self.lib = ctypes.CDLL('./SpeechCore.dylib')
        else:
            raise OSError("Unsupported platform")

        self._setup_function_prototypes()

    def _setup_function_prototypes(self):
        self.lib.Speech_Init.argtypes = []
        self.lib.Speech_Init.restype = None

        self.lib.Speech_Free.argtypes = []
        self.lib.Speech_Free.restype = None

        self.lib.Speech_Detect_Driver.argtypes = []
        self.lib.Speech_Detect_Driver.restype = None

        self.lib.Speech_Current_Driver.argtypes = []
        self.lib.Speech_Current_Driver.restype = ctypes.c_wchar_p

        self.lib.Get_Speech_Flags.argtypes = []
        self.lib.Get_Speech_Flags.restype = ctypes.c_uint32
        self.lib.Speech_Output.argtypes = [ctypes.c_wchar_p, ctypes.c_bool]
        self.lib.Speech_Output.restype = ctypes.c_bool

        self.lib.Speech_Stop.argtypes = []
        self.lib.Speech_Stop.restype = ctypes.c_bool

        self.lib.Speech_Is_Loaded.argtypes = []
        self.lib.Speech_Is_Loaded.restype = ctypes.c_bool

        self.lib.Speech_Is_speaking.argtypes = []
        self.lib.Speech_Is_speaking.restype = ctypes.c_bool

        self.lib.Speech_Get_Driver.argtypes = [ctypes.c_int]
        self.lib.Speech_Get_Driver.restype = ctypes.c_wchar_p

        self.lib.Speech_Set_Driver.argtypes = [ctypes.c_int]
        self.lib.Speech_Set_Driver.restype = None

        self.lib.Speech_Get_Drivers.argtypes = []
        self.lib.Speech_Get_Drivers.restype = ctypes.c_int

        self.lib.Speech_Braille.argtypes = [ctypes.c_wchar_p]
        self.lib.Speech_Braille.restype = ctypes.c_bool

        self.lib.Speech_Get_Volume.argtypes = []
        self.lib.Speech_Get_Volume.restype = ctypes.c_float

        self.lib.Speech_Set_Volume.argtypes = [ctypes.c_float]
        self.lib.Speech_Set_Volume.restype = None

        self.lib.Speech_Get_Rate.argtypes = []
        self.lib.Speech_Get_Rate.restype = ctypes.c_float

        self.lib.Speech_Set_Rate.argtypes = [ctypes.c_float]
        self.lib.Speech_Set_Rate.restype = None

        self.lib.Speech_Get_Current_Voice.argtypes = []
        self.lib.Speech_Get_Current_Voice.restype = ctypes.c_wchar_p

        self.lib.Speech_Get_Voice.argtypes = [ctypes.c_int]
        self.lib.Speech_Get_Voice.restype = ctypes.c_wchar_p

        self.lib.Speech_Set_voice.argtypes = [ctypes.c_int]
        self.lib.Speech_Set_voice.restype = None

        self.lib.Speech_Get_Voices.argtypes = []
        self.lib.Speech_Get_Voices.restype = ctypes.c_int

        self.lib.Speech_Output_File.argtypes = [ctypes.c_char_p, ctypes.c_wchar_p]
        self.lib.Speech_Output_File.restype = None
        self.lib.Speech_Resume.argtypes = []
        self.lib.Speech_Resume.restype = None

        self.lib.Speech_pause.argtypes = []
        self.lib.Speech_pause.restype = None

    def _setup_sapi_function_prototypes(self):
        self.lib.Speech_Sapi_loaded.argtypes = []
        self.lib.Speech_Sapi_loaded.restype = ctypes.c_bool
        self.lib.Speech_Prefer_Sapi.argtypes = [ctypes.c_bool]
        self.lib.Speech_Prefer_Sapi.restype = None
        self.lib.Sapi_Init.argtypes = []
        self.lib.Sapi_Init.restype = None

        self.lib.Sapi_Release.argtypes = []
        self.lib.Sapi_Release.restype = None

        self.lib.Sapi_Get_Current_Voice.argtypes = []
        self.lib.Sapi_Get_Current_Voice.restype = ctypes.c_wchar_p

        self.lib.Sapi_Get_Voice.argtypes = [ctypes.c_int]
        self.lib.Sapi_Get_Voice.restype = ctypes.c_wchar_p

        self.lib.Sapi_Set_Voice.argtypes = [ctypes.c_wchar_p]
        self.lib.Sapi_Set_Voice.restype = None

        self.lib.Sapi_Set_Voice_By_Index.argtypes = [ctypes.c_int]
        self.lib.Sapi_Set_Voice_By_Index.restype = None

        self.lib.Sapi_Get_Voices.argtypes = []
        self.lib.Sapi_Get_Voices.restype = ctypes.c_int

        self.lib.Sapi_Voice_Get_Volume.argtypes = []
        self.lib.Sapi_Voice_Get_Volume.restype = ctypes.c_float

        self.lib.Sapi_Voice_Set_Volume.argtypes = [ctypes.c_float]
        self.lib.Sapi_Voice_Set_Volume.restype = None

        self.lib.Sapi_Voice_Get_Rate.argtypes = []
        self.lib.Sapi_Voice_Get_Rate.restype = ctypes.c_float

        self.lib.Sapi_Voice_Set_Rate.argtypes = [ctypes.c_float]
        self.lib.Sapi_Voice_Set_Rate.restype = None

        self.lib.Sapi_Speak.argtypes = [ctypes.c_wchar_p, ctypes.c_bool, ctypes.c_bool]
        self.lib.Sapi_Speak.restype = None

        self.lib.Sapi_Output_File.argtypes = [ctypes.c_char_p, ctypes.c_wchar_p, ctypes.c_bool]
        self.lib.Sapi_Output_File.restype = None

        self.lib.Sapi_Pause.argtypes = []
        self.lib.Sapi_Pause.restype = None

        self.lib.Sapi_Resume.argtypes = []
        self.lib.Sapi_Resume.restype = None

        self.lib.Sapi_Stop.argtypes = []
        self.lib.Sapi_Stop.restype = None

    def init(self):
        self.lib.Speech_Init()

    def free(self):
        self.lib.Speech_Free()

    def speak(self, text, interrupt=False):
        return self.lib.Speech_Output(text, interrupt)

    def braille(self, text):
        return self.lib.Speech_Braille(text)

    def stop(self):
        return self.lib.Speech_Stop()

    def get_volume(self):
        return self.lib.Speech_Get_Volume()

    def set_volume(self, volume):
        self.lib.Speech_Set_Volume(volume)

    def get_rate(self):
        return self.lib.Speech_Get_Rate()

    def set_rate(self, rate):
        self.lib.Speech_Set_Rate(rate)

    def get_current_voice(self):
        return self.lib.Speech_Get_Current_Voice()

    def set_voice(self, index):
        self.lib.Speech_Set_voice(index)

    def get_voice_count(self):
        return self.lib.Speech_Get_Voices()

    def output_to_file(self, file_path, text):
        self.lib.Speech_Output_File(file_path.encode('utf-8'), text)

    def detect_driver(self):
        self.lib.Speech_Detect_Driver()

    def current_driver(self):
        return self.lib.Speech_Current_Driver()

    def get_driver(self, index):
        return self.lib.Speech_Get_Driver(index)

    def set_driver(self, index):
        self.lib.Speech_Set_Driver(index)

    def get_drivers(self):
        return self.lib.Speech_Get_Drivers()

    def get_speech_flags(self):
        return self.lib.Get_Speech_Flags()

    def check_speech_flags(self, flags):
        return bool(self.get_speech_flags() & flags)

    def is_loaded(self):
        return self.lib.Speech_Is_Loaded()

    def is_speaking(self):
        return self.lib.Speech_Is_speaking()

    def get_voice(self, index):
        return self.lib.Speech_Get_Voice(index)

    def resume(self):
        self.lib.Speech_Resume()

    def pause(self):
        self.lib.Speech_pause()

    def sapi_init(self):
        if self._has_sapi:
                self.lib.Sapi_Init()
        else:
                raise SAPI_ERROR

    def sapi_release(self):
        if self._has_sapi:
                self.lib.Sapi_Release()
        else:
                raise SAPI_ERROR

    def sapi_get_current_voice(self):
        if self._has_sapi:
                return self.lib.Sapi_Get_Current_Voice()
        else:
                raise SAPI_ERROR

    def sapi_get_voice(self, index):
        if self._has_sapi:
                return self.lib.Sapi_Get_Voice(index)
        else:
                raise SAPI_ERROR

    def sapi_set_voice(self, voice_name):
        if self._has_sapi:
                self.lib.Sapi_Set_Voice(voice_name)
        else:
                raise SAPI_ERROR

    def sapi_set_voice_by_index(self, index):
        if self._has_sapi:
                self.lib.Sapi_Set_Voice_By_Index(index)
        else:
                raise SAPI_ERROR

    def sapi_get_voices(self):
        if self._has_sapi:
                return self.lib.Sapi_Get_Voices()
        else:
                raise SAPI_ERROR

    def sapi_voice_get_volume(self):
        if self._has_sapi:
                return self.lib.Sapi_Voice_Get_Volume()
        else:
                raise SAPI_ERROR

    def sapi_voice_set_volume(self, volume):
        if self._has_sapi:
                self.lib.Sapi_Voice_Set_Volume(volume)
        else:
                raise SAPI_ERROR

    def sapi_voice_get_rate(self):
        if self._has_sapi:
                return self.lib.Sapi_Voice_Get_Rate()
        else:
                raise SAPI_ERROR

    def sapi_voice_set_rate(self, rate):
        if self._has_sapi:
                self.lib.Sapi_Voice_Set_Rate(rate)
        else:
                raise SAPI_ERROR

    def sapi_speak(self, text, interrupt=False, xml = False):
        if self._has_sapi:
                self.lib.Sapi_Speak(text, interrupt, xml)
        else:
                raise SAPI_ERROR

    def sapi_output_file(self, file_path, text, xml=False):
        if self._has_sapi:
                self.lib.Sapi_Output_File(file_path.encode('utf-8'), text, xml)
        else:
                raise SAPI_ERROR

    def sapi_pause(self):
        if self._has_sapi:
                self.lib.Sapi_Pause()
        else:
                raise SAPI_ERROR

    def sapi_resume(self):
        if self._has_sapi:
                self.lib.Sapi_Resume()
        else:
                raise SAPI_ERROR

    def sapi_stop(self):
        if self._has_sapi:
                self.lib.Sapi_Stop()
        else:
                raise SAPI_ERROR
    def prefer_sapi(self, prefer_sapi):
        if self._has_sapi:
            return self.lib.Speech_Prefer_Sapi(prefer_sapi)
        else:
            raise self.SAPI_ERROR

    def sapi_loaded(self):
        if self._has_sapi:
            return self.lib.Speech_Sapi_loaded()
        else:
            raise self.SAPI_ERROR
