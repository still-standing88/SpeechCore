using System;
using System.Runtime.InteropServices;

public class SpeechCore
{
    private const string DllName = "SpeechCore";

    public const uint SC_SPEECH_FLOW_CONTROL = 1 << 0;
    public const uint SC_SPEECH_PARAMETER_CONTROL = 1 << 1;
    public const uint SC_VOICE_CONFIG = 1 << 2;
    public const uint SC_FILE_OUTPUT = 1 << 3;
    public const uint SC_HAS_SPEECH = 1 << 4;
    public const uint SC_HAS_BRAILLE = 1 << 5;

    [DllImport(DllName)]
    private static extern void Speech_Init();

    [DllImport(DllName)]
    private static extern void Speech_Free();

    [DllImport(DllName)]
    private static extern void Speech_Detect_Driver();

    [DllImport(DllName)]
    private static extern IntPtr Speech_Current_Driver();

[DllImport(DllName)]
private static extern IntPtr Speech_Get_Driver(int index);

[DllImport(DllName)]
private static extern void Speech_Set_Driver(int index);

[DllImport(DllName)]
private static extern int Speech_Get_Drivers();

    [DllImport(DllName)]
    private static extern uint Get_Speech_Flags();

    [DllImport(DllName)]
    [return: MarshalAs(UnmanagedType.Bool)]
    private static extern bool Speech_Is_Loaded();

    [DllImport(DllName)]
    [return: MarshalAs(UnmanagedType.Bool)]
    private static extern bool Speech_Is_speaking();

    [DllImport(DllName)]
    [return: MarshalAs(UnmanagedType.Bool)]
    private static extern bool Speech_Output([MarshalAs(UnmanagedType.LPWStr)] string text, [MarshalAs(UnmanagedType.Bool)] bool interrupt = false);

[DllImport(DllName)]
[return: MarshalAs(UnmanagedType.Bool)]
private static extern bool Speech_Braille([MarshalAs(UnmanagedType.LPWStr)] string text);

    [DllImport(DllName)]
    [return: MarshalAs(UnmanagedType.Bool)]
    private static extern bool Speech_Stop();

    [DllImport(DllName)]
    private static extern float Speech_Get_Volume();

    [DllImport(DllName)]
    private static extern void Speech_Set_Volume(float offset);

    [DllImport(DllName)]
    private static extern float Speech_Get_Rate();

    [DllImport(DllName)]
    private static extern void Speech_Set_Rate(float offset);

    [DllImport(DllName)]
    private static extern IntPtr Speech_Get_Current_Voice();

    [DllImport(DllName)]
    private static extern IntPtr Speech_Get_Voice(int index);

    [DllImport(DllName)]
    private static extern void Speech_Set_voice(int index);

    [DllImport(DllName)]
    private static extern int Speech_Get_Voices();

    [DllImport(DllName)]
    private static extern void Speech_Output_File([MarshalAs(UnmanagedType.LPStr)] string filePath, [MarshalAs(UnmanagedType.LPWStr)] string text);

    [DllImport(DllName)]
    private static extern void Speech_Resume();

    [DllImport(DllName)]
    private static extern void Speech_pause();

    #if _WIN32
    [DllImport(DllName)]
    private static extern void Speech_Prefer_Sapi([MarshalAs(UnmanagedType.Bool)] bool prefer_sapi);

    [DllImport(DllName)]
    private static extern void Sapi_Init();

    [DllImport(DllName)]
    private static extern void Sapi_Release();

    [DllImport(DllName)]
    private static extern IntPtr Sapi_Get_Current_Voice();

    [DllImport(DllName)]
    private static extern IntPtr Sapi_Get_Voice(int index);

    [DllImport(DllName)]
    private static extern void Sapi_Set_Voice([MarshalAs(UnmanagedType.LPWStr)] string voice);

    [DllImport(DllName)]
    private static extern void Sapi_Set_Voice_By_Index(int index);

    [DllImport(DllName)]
    private static extern int Sapi_Get_Voices();

    [DllImport(DllName)]
    private static extern float Sapi_Voice_Get_Volume();

    [DllImport(DllName)]
    private static extern void Sapi_Voice_Set_Volume(float volume);

    [DllImport(DllName)]
    private static extern float Sapi_Voice_Get_Rate();

    [DllImport(DllName)]
    private static extern void Sapi_Voice_Set_Rate(float rate);

    [DllImport(DllName)]
    private static extern void Sapi_Speak([MarshalAs(UnmanagedType.LPWStr)] string text, [MarshalAs(UnmanagedType.Bool)] bool interrupt = false, [MarshalAs(UnmanagedType.Bool)] bool xml = false);

    [DllImport(DllName)]
    private static extern void Sapi_Output_File([MarshalAs(UnmanagedType.LPStr)] string filename, [MarshalAs(UnmanagedType.LPWStr)] string text, [MarshalAs(UnmanagedType.Bool)] bool xml = false);

    [DllImport(DllName)]
    private static extern void Sapi_Pause();

    [DllImport(DllName)]
    private static extern void Sapi_Resume();

    [DllImport(DllName)]
    private static extern void Sapi_Stop();
    #endif

    public SpeechCore()
    {
        Speech_Init();
    }

    ~SpeechCore()
    {
        Speech_Free();
    }

    public void DetectDriver() => Speech_Detect_Driver();

    public string CurrentDriver() => Marshal.PtrToStringUni(Speech_Current_Driver());

public string GetDriver(int index) => Marshal.PtrToStringUni(Speech_Get_Driver(index));

public void SetDriver(int index) => Speech_Set_Driver(index);

public int GetDrivers() => Speech_Get_Drivers();

    public uint GetSpeechFlags() => Get_Speech_Flags();

    public bool IsLoaded() => Speech_Is_Loaded();

    public bool IsSpeaking() => Speech_Is_speaking();

    public bool Speak(string text, bool interrupt = false) => Speech_Output(text, interrupt);

public bool Braille(string text) => Speech_Braille(text);

    public bool Stop() => Speech_Stop();

    public float GetVolume() => Speech_Get_Volume();

    public void SetVolume(float volume) => Speech_Set_Volume(volume);

    public float GetRate() => Speech_Get_Rate();

    public void SetRate(float rate) => Speech_Set_Rate(rate);

    public string GetCurrentVoice() => Marshal.PtrToStringUni(Speech_Get_Current_Voice());

    public string GetVoice(int index) => Marshal.PtrToStringUni(Speech_Get_Voice(index));

    public void SetVoice(int index) => Speech_Set_voice(index);

    public int GetVoiceCount() => Speech_Get_Voices();

    public void OutputToFile(string filePath, string text) => Speech_Output_File(filePath, text);

    public void Resume() => Speech_Resume();

    public void Pause() => Speech_pause();

    public bool CheckSpeechFlags(uint flags) => (GetSpeechFlags() & flags) != 0;

    #if _WIN32
    public void PreferSapi(bool preferSapi) => Speech_Prefer_Sapi(preferSapi);

    public void SapiInit() => Sapi_Init();

    public void SapiRelease() => Sapi_Release();

    public string SapiGetCurrentVoice() => Marshal.PtrToStringUni(Sapi_Get_Current_Voice());

    public string SapiGetVoice(int index) => Marshal.PtrToStringUni(Sapi_Get_Voice(index));

    public void SapiSetVoice(string voice) => Sapi_Set_Voice(voice);

    public void SapiSetVoiceByIndex(int index) => Sapi_Set_Voice_By_Index(index);

    public int SapiGetVoices() => Sapi_Get_Voices();

    public float SapiGetVolume() => Sapi_Voice_Get_Volume();

    public void SapiSetVolume(float volume) => Sapi_Voice_Set_Volume(volume);

    public float SapiGetRate() => Sapi_Voice_Get_Rate();

    public void SapiSetRate(float rate) => Sapi_Voice_Set_Rate(rate);

    public void SapiSpeak(string text, bool interrupt = false, bool xml = false) => Sapi_Speak(text, interrupt, xml);

    public void SapiOutputFile(string filename, string text, bool xml = false) => Sapi_Output_File(filename, text, xml);

    public void SapiPause() => Sapi_Pause();

    public void SapiResume() => Sapi_Resume();

    public void SapiStop() => Sapi_Stop();
    #endif
}