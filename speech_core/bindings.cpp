#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/functional.h>

#include <string>
#include <vector>
#include <cstring>

#if defined(__linux__) || defined(__APPLE__) || defined(__DARWIN__) || defined(__MACH__)
#include <locale>
#include <codecvt>
#endif

#include "SpeechCore.h"

namespace py = pybind11;

std::string wchar_to_string(const wchar_t* wstr) {
    if (!wstr) return "";
    
#if defined(__linux__) || defined(__APPLE__) || defined(__DARWIN__) || defined(__MACH__)
    return std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(wstr);
#else
    std::wstring ws(wstr);
    return std::string(ws.begin(), ws.end());
#endif
}

std::wstring string_to_wstring(const std::string& str) {
#if defined(__linux__) || defined(__APPLE__) || defined(__DARWIN__) || defined(__MACH__)
    return std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(str);
#else
    return std::wstring(str.begin(), str.end());
#endif
}

PYBIND11_MODULE(SpeechCore, m) {
    m.doc() = "Python bindings for the SpeechCore cross-platform screen reader library";

    m.attr("SC_SPEECH_FLOW_CONTROL") = py::int_(SC_SPEECH_FLOW_CONTROL);
    m.attr("SC_SPEECH_PARAMETER_CONTROL") = py::int_(SC_SPEECH_PARAMETER_CONTROL);
    m.attr("SC_VOICE_CONFIG") = py::int_(SC_VOICE_CONFIG);
    m.attr("SC_FILE_OUTPUT") = py::int_(SC_FILE_OUTPUT);
    m.attr("SC_HAS_SPEECH") = py::int_(SC_HAS_SPEECH);
    m.attr("SC_HAS_BRAILLE") = py::int_(SC_HAS_BRAILLE);
    m.attr("SC_HAS_SPEECH_STATE") = py::int_(SC_HAS_SPEECH_STATE);
    m.attr("SC_SSML_SUPPORT") = py::int_(SC_SSML_SUPPORT);

    m.def("init", &Speech_Init);
    m.def("free", &Speech_Free);
    m.def("detect_driver", &Speech_Detect_Driver);
    
    m.def("current_driver", []() -> py::object {
        const wchar_t* driver = Speech_Current_Driver();
        if (driver) {
            return py::str(wchar_to_string(driver));
        }
        return py::none();
    });
    
    m.def("get_driver", [](int index) -> py::object {
        const wchar_t* driver = Speech_Get_Driver(index);
        if (driver) {
            return py::str(wchar_to_string(driver));
        }
        return py::none();
    }, py::arg("index"));
    
    m.def("set_driver", &Speech_Set_Driver, py::arg("index"));
    m.def("get_drivers", &Speech_Get_Drivers);
    m.def("get_flags", &Speech_Get_Flags);
    m.def("is_loaded", &Speech_Is_Loaded);
    m.def("is_speaking", &Speech_Is_Speaking);
    
    m.def("output", [](const std::string& text, bool interrupt = false) -> bool {
        static thread_local std::wstring wtext_holder;
        wtext_holder = string_to_wstring(text);
        return Speech_Output(wtext_holder.c_str(), interrupt);
    }, py::arg("text"), py::arg("interrupt") = false);
    
    m.def("output_text", [](const std::string& text, bool interrupt = false, bool with_ssml = false) -> bool {
        static thread_local std::wstring wtext_holder;
        wtext_holder = string_to_wstring(text);
        return Speech_Output_text(wtext_holder.c_str(), interrupt, with_ssml);
    }, py::arg("text"), py::arg("interrupt") = false, py::arg("with_ssml") = false);
    
    m.def("braille", [](const std::string& text) -> bool {
        static thread_local std::wstring wtext_holder;
        wtext_holder = string_to_wstring(text);
        return Speech_Braille(wtext_holder.c_str());
    }, py::arg("text"));
    
    m.def("stop", &Speech_Stop);
    m.def("get_volume", &Speech_Get_Volume);
    m.def("set_volume", &Speech_Set_Volume, py::arg("volume"));
    m.def("get_rate", &Speech_Get_Rate);
    m.def("set_rate", &Speech_Set_Rate, py::arg("rate"));
    m.def("get_pitch", &Speech_Get_Pitch);
    m.def("set_pitch", &Speech_Set_Pitch, py::arg("pitch"));
    
    m.def("get_current_voice", []() -> py::object {
        const wchar_t* voice = Speech_Get_Current_Voice();
        if (voice) {
            return py::str(wchar_to_string(voice));
        }
        return py::none();
    });
    
    m.def("get_voice", [](int index) -> py::object {
        const wchar_t* voice = Speech_Get_Voice(index);
        if (voice) {
            return py::str(wchar_to_string(voice));
        }
        return py::none();
    }, py::arg("index"));
    
    m.def("set_voice", &Speech_Set_Voice, py::arg("index"));
    m.def("get_voices", &Speech_Get_Voices);
    
    m.def("output_file", [](const std::string& file_path, const std::string& text) {
        static thread_local std::wstring wtext_holder;
        wtext_holder = string_to_wstring(text);
        Speech_Output_File(file_path.c_str(), wtext_holder.c_str());
    }, py::arg("file_path"), py::arg("text"));
    
    m.def("resume", &Speech_Resume);
    m.def("pause", &Speech_Pause);

#ifdef _WIN32
    m.def("prefer_sapi", &Speech_Prefer_Sapi, py::arg("prefer_sapi"));
    m.def("sapi_loaded", &Speech_Sapi_Loaded);
    m.def("sapi_init", &Sapi_Init);
    m.def("sapi_release", &Sapi_Release);
    
    m.def("sapi_get_current_voice", []() -> py::object {
        const wchar_t* voice = Sapi_Get_Current_Voice();
        if (voice) {
            return py::str(wchar_to_string(voice));
        }
        return py::none();
    });
    
    m.def("sapi_get_voice", [](int index) -> py::object {
        const wchar_t* voice = Sapi_Get_Voice(index);
        if (voice) {
            return py::str(wchar_to_string(voice));
        }
        return py::none();
    }, py::arg("index"));
    
    m.def("sapi_set_voice", [](const std::string& voice) {
        static thread_local std::wstring wvoice_holder;
        wvoice_holder = string_to_wstring(voice);
        Sapi_Set_Voice(wvoice_holder.c_str());
    }, py::arg("voice"));
    
    m.def("sapi_set_voice_by_index", &Sapi_Set_Voice_By_Index, py::arg("index"));
    m.def("sapi_get_voices", &Sapi_Get_Voices);
    m.def("sapi_voice_get_volume", &Sapi_Voice_Get_Volume);
    m.def("sapi_voice_set_volume", &Sapi_Voice_Set_Volume, py::arg("volume"));
    m.def("sapi_voice_get_rate", &Sapi_Voice_Get_Rate);
    m.def("sapi_voice_set_rate", &Sapi_Voice_Set_Rate, py::arg("rate"));
    
    m.def("sapi_speak", [](const std::string& text, bool interrupt = false, bool xml = false) {
        static thread_local std::wstring wtext_holder;
        wtext_holder = string_to_wstring(text);
        Sapi_Speak(wtext_holder.c_str(), interrupt, xml);
    }, py::arg("text"), py::arg("interrupt") = false, py::arg("xml") = false);
    
    m.def("sapi_output_file", [](const std::string& filename, const std::string& text, bool xml = false) {
        static thread_local std::wstring wtext_holder;
        wtext_holder = string_to_wstring(text);
        Sapi_Output_File(filename.c_str(), wtext_holder.c_str(), xml);
    }, py::arg("filename"), py::arg("text"), py::arg("xml") = false);
    
    m.def("sapi_pause", &Sapi_Pause);
    m.def("sapi_resume", &Sapi_Resume);
    m.def("sapi_stop", &Sapi_Stop);
#endif

#ifdef VERSION_INFO
    m.attr("__version__") = PYBIND11_TOSTRING(VERSION_INFO);
#else
    m.attr("__version__") = "1.1.0";
#endif
}