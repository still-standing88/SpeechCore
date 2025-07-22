using System;
using System.Runtime.InteropServices;
using System.Text;
using System.Collections.Generic;

public class SpeechCore : IDisposable
{
    private bool disposed = false;
    private const string DllName = "SpeechCore";

    public const uint SC_SPEECH_FLOW_CONTROL = 1 << 0;
    public const uint SC_SPEECH_PARAMETER_CONTROL = 1 << 1;
    public const uint SC_VOICE_CONFIG = 1 << 2;
    public const uint SC_FILE_OUTPUT = 1 << 3;
    public const uint SC_HAS_SPEECH = 1 << 4;
    public const uint SC_HAS_BRAILLE = 1 << 5;
    public const uint SC_HAS_SPEECH_STATE = 1 << 6;
    public const uint SC_SSML_SUPPORT = 1 << 7;

    private static readonly bool IsWindows = RuntimeInformation.IsOSPlatform(OSPlatform.Windows);

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
    private static extern uint Speech_Get_Flags();

    [DllImport(DllName)]
    [return: MarshalAs(UnmanagedType.Bool)]
    private static extern bool Speech_Is_Loaded();

    [DllImport(DllName)]
    [return: MarshalAs(UnmanagedType.Bool)]
    private static extern bool Speech_Is_Speaking();

    [DllImport(DllName, EntryPoint = "Speech_Output")]
    [return: MarshalAs(UnmanagedType.Bool)]
    private static extern bool Speech_Output_Windows([MarshalAs(UnmanagedType.LPWStr)] string text, [MarshalAs(UnmanagedType.Bool)] bool interrupt);

    [DllImport(DllName, EntryPoint = "Speech_Output")]
    [return: MarshalAs(UnmanagedType.Bool)]
    private static extern bool Speech_Output_Unix(IntPtr textPtr, [MarshalAs(UnmanagedType.Bool)] bool interrupt);

    [DllImport(DllName, EntryPoint = "Speech_Output_text")]
    [return: MarshalAs(UnmanagedType.Bool)]
    private static extern bool Speech_Output_text_Windows([MarshalAs(UnmanagedType.LPWStr)] string text, [MarshalAs(UnmanagedType.Bool)] bool interrupt, [MarshalAs(UnmanagedType.Bool)] bool with_ssml);

    [DllImport(DllName, EntryPoint = "Speech_Output_text")]
    [return: MarshalAs(UnmanagedType.Bool)]
    private static extern bool Speech_Output_text_Unix(IntPtr textPtr, [MarshalAs(UnmanagedType.Bool)] bool interrupt, [MarshalAs(UnmanagedType.Bool)] bool with_ssml);

    [DllImport(DllName, EntryPoint = "Speech_Braille")]
    [return: MarshalAs(UnmanagedType.Bool)]
    private static extern bool Speech_Braille_Windows([MarshalAs(UnmanagedType.LPWStr)] string text);

    [DllImport(DllName, EntryPoint = "Speech_Braille")]
    [return: MarshalAs(UnmanagedType.Bool)]
    private static extern bool Speech_Braille_Unix(IntPtr textPtr);

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
    private static extern float Speech_Get_Pitch();

    [DllImport(DllName)]
    private static extern void Speech_Set_Pitch(float offset);

    [DllImport(DllName)]
    private static extern IntPtr Speech_Get_Current_Voice();

    [DllImport(DllName)]
    private static extern IntPtr Speech_Get_Voice(int index);

    [DllImport(DllName)]
    private static extern void Speech_Set_Voice(int index);

    [DllImport(DllName)]
    private static extern int Speech_Get_Voices();

    [DllImport(DllName, EntryPoint = "Speech_Output_File")]
    private static extern void Speech_Output_File_Windows([MarshalAs(UnmanagedType.LPStr)] string filePath, [MarshalAs(UnmanagedType.LPWStr)] string text);

    [DllImport(DllName, EntryPoint = "Speech_Output_File")]
    private static extern void Speech_Output_File_Unix([MarshalAs(UnmanagedType.LPStr)] string filePath, IntPtr textPtr);

    [DllImport(DllName)]
    private static extern void Speech_Resume();

    [DllImport(DllName)]
    private static extern void Speech_Pause();

    [DllImport(DllName)]
    private static extern void Speech_Prefer_Sapi([MarshalAs(UnmanagedType.Bool)] bool prefer_sapi);

    [DllImport(DllName)]
    [return: MarshalAs(UnmanagedType.Bool)]
    private static extern bool Speech_Sapi_Loaded();

    [DllImport(DllName)]
    private static extern void Sapi_Init();

    [DllImport(DllName)]
    private static extern void Sapi_Release();

    [DllImport(DllName)]
    private static extern IntPtr Sapi_Get_Current_Voice();

    [DllImport(DllName)]
    private static extern IntPtr Sapi_Get_Voice(int index);

    [DllImport(DllName, EntryPoint = "Sapi_Set_Voice")]
    private static extern void Sapi_Set_Voice_Windows([MarshalAs(UnmanagedType.LPWStr)] string voice);

    [DllImport(DllName, EntryPoint = "Sapi_Set_Voice")]
    private static extern void Sapi_Set_Voice_Unix(IntPtr voicePtr);

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

    [DllImport(DllName, EntryPoint = "Sapi_Speak")]
    private static extern void Sapi_Speak_Windows([MarshalAs(UnmanagedType.LPWStr)] string text, [MarshalAs(UnmanagedType.Bool)] bool interrupt, [MarshalAs(UnmanagedType.Bool)] bool xml);

    [DllImport(DllName, EntryPoint = "Sapi_Speak")]
    private static extern void Sapi_Speak_Unix(IntPtr textPtr, [MarshalAs(UnmanagedType.Bool)] bool interrupt, [MarshalAs(UnmanagedType.Bool)] bool xml);

    [DllImport(DllName, EntryPoint = "Sapi_Output_File")]
    private static extern void Sapi_Output_File_Windows([MarshalAs(UnmanagedType.LPStr)] string filename, [MarshalAs(UnmanagedType.LPWStr)] string text, [MarshalAs(UnmanagedType.Bool)] bool xml);

    [DllImport(DllName, EntryPoint = "Sapi_Output_File")]
    private static extern void Sapi_Output_File_Unix([MarshalAs(UnmanagedType.LPStr)] string filename, IntPtr textPtr, [MarshalAs(UnmanagedType.Bool)] bool xml);

    [DllImport(DllName)]
    private static extern void Sapi_Pause();

    [DllImport(DllName)]
    private static extern void Sapi_Resume();

    [DllImport(DllName)]
    private static extern void Sapi_Stop();

    public SpeechCore()
    {
        Speech_Init();
    }

    public void Dispose()
    {
        Dispose(true);
        GC.SuppressFinalize(this);
    }

    protected virtual void Dispose(bool disposing)
    {
        if (!disposed)
        {
            Speech_Free();
            disposed = true;
        }
    }

    ~SpeechCore()
    {
        Dispose(false);
    }

    private IntPtr StringToWChar(string text)
    {
        if (string.IsNullOrEmpty(text))
            return IntPtr.Zero;

        byte[] bytes;
        if (IsWindows)
        {

            bytes = Encoding.Unicode.GetBytes(text + "\0");
        }
        else
        {
            bytes = Encoding.UTF32.GetBytes(text + "\0");
        }

        IntPtr ptr = Marshal.AllocHGlobal(bytes.Length);
        Marshal.Copy(bytes, 0, ptr, bytes.Length);
        return ptr;
    }

    
    private string WCharToString(IntPtr ptr)
    {
        if (ptr == IntPtr.Zero)
            return null;

        if (IsWindows)
        {
            return Marshal.PtrToStringUni(ptr);
        }
        else
        {
            var bytes = new List<byte>();
            int offset = 0;

            while (true)
            {
                byte[] charBytes = new byte[4];
                Marshal.Copy(ptr + offset, charBytes, 0, 4);

                if (charBytes[0] == 0 && charBytes[1] == 0 && charBytes[2] == 0 && charBytes[3] == 0)
                    break;

                bytes.AddRange(charBytes);
                offset += 4;
            }

            if (bytes.Count == 0)
                return string.Empty;

            return Encoding.UTF32.GetString(bytes.ToArray());
        }
    }

    public void DetectDriver() => Speech_Detect_Driver();

    public string CurrentDriver() => WCharToString(Speech_Current_Driver());

    public string GetDriver(int index) => WCharToString(Speech_Get_Driver(index));

    public void SetDriver(int index) => Speech_Set_Driver(index);

    public int GetDrivers() => Speech_Get_Drivers();

    public uint GetSpeechFlags() => Speech_Get_Flags();

    public bool IsLoaded() => Speech_Is_Loaded();

    public bool IsSpeaking() => Speech_Is_Speaking();

    public bool Speak(string text, bool interrupt = false)
    {
        if (string.IsNullOrEmpty(text))
            return false;

        if (IsWindows)
        {
            return Speech_Output_Windows(text, interrupt);
        }
        else
        {
            IntPtr textPtr = StringToWChar(text);
            try
            {
                return Speech_Output_Unix(textPtr, interrupt);
            }
            finally
            {
                if (textPtr != IntPtr.Zero)
                    Marshal.FreeHGlobal(textPtr);
            }
        }
    }

    public bool SpeakWithSSML(string text, bool interrupt = false, bool with_ssml = false)
    {
        if (string.IsNullOrEmpty(text))
            return false;

        if (IsWindows)
        {
            return Speech_Output_text_Windows(text, interrupt, with_ssml);
        }
        else
        {
            IntPtr textPtr = StringToWChar(text);
            try
            {
                return Speech_Output_text_Unix(textPtr, interrupt, with_ssml);
            }
            finally
            {
                if (textPtr != IntPtr.Zero)
                    Marshal.FreeHGlobal(textPtr);
            }
        }
    }

    public bool Braille(string text)
    {
        if (string.IsNullOrEmpty(text))
            return false;

        if (IsWindows)
        {
            return Speech_Braille_Windows(text);
        }
        else
        {
            IntPtr textPtr = StringToWChar(text);
            try
            {
                return Speech_Braille_Unix(textPtr);
            }
            finally
            {
                if (textPtr != IntPtr.Zero)
                    Marshal.FreeHGlobal(textPtr);
            }
        }
    }

    public bool Stop() => Speech_Stop();

    public float GetVolume() => Speech_Get_Volume();

    public void SetVolume(float volume) => Speech_Set_Volume(volume);

    public float GetRate() => Speech_Get_Rate();

    public void SetRate(float rate) => Speech_Set_Rate(rate);

    public float GetPitch() => Speech_Get_Pitch();

    public void SetPitch(float pitch) => Speech_Set_Pitch(pitch);

    public string GetCurrentVoice() => WCharToString(Speech_Get_Current_Voice());

    public string GetVoice(int index) => WCharToString(Speech_Get_Voice(index));

    public void SetVoice(int index) => Speech_Set_Voice(index);

    public int GetVoiceCount() => Speech_Get_Voices();

    public void OutputToFile(string filePath, string text)
    {
        if (IsWindows)
        {
            Speech_Output_File_Windows(filePath, text);
        }
        else
        {
            IntPtr textPtr = StringToWChar(text);
            try
            {
                Speech_Output_File_Unix(filePath, textPtr);
            }
            finally
            {
                if (textPtr != IntPtr.Zero)
                    Marshal.FreeHGlobal(textPtr);
            }
        }
    }

    public void Resume() => Speech_Resume();

    public void Pause() => Speech_Pause();

    public bool CheckSpeechFlags(uint flags) => (GetSpeechFlags() & flags) != 0;

    public void PreferSapi(bool preferSapi) => Speech_Prefer_Sapi(preferSapi);

    public bool SapiLoaded() => Speech_Sapi_Loaded();

    public void SapiInit() => Sapi_Init();

    public void SapiRelease() => Sapi_Release();

    public string SapiGetCurrentVoice() => WCharToString(Sapi_Get_Current_Voice());

    public string SapiGetVoice(int index) => WCharToString(Sapi_Get_Voice(index));

    public void SapiSetVoice(string voice)
    {
        if (IsWindows)
        {
            Sapi_Set_Voice_Windows(voice);
        }
        else
        {
            IntPtr voicePtr = StringToWChar(voice);
            try
            {
                Sapi_Set_Voice_Unix(voicePtr);
            }
            finally
            {
                if (voicePtr != IntPtr.Zero)
                    Marshal.FreeHGlobal(voicePtr);
            }
        }
    }

    public void SapiSetVoiceByIndex(int index) => Sapi_Set_Voice_By_Index(index);

    public int SapiGetVoices() => Sapi_Get_Voices();

    public float SapiVoiceGetVolume() => Sapi_Voice_Get_Volume();

    public void SapiVoiceSetVolume(float volume) => Sapi_Voice_Set_Volume(volume);

    public float SapiVoiceGetRate() => Sapi_Voice_Get_Rate();

    public void SapiVoiceSetRate(float rate) => Sapi_Voice_Set_Rate(rate);

    public void SapiSpeak(string text, bool interrupt = false, bool xml = false)
    {
        if (IsWindows)
        {
            Sapi_Speak_Windows(text, interrupt, xml);
        }
        else
        {
            IntPtr textPtr = StringToWChar(text);
            try
            {
                Sapi_Speak_Unix(textPtr, interrupt, xml);
            }
            finally
            {
                if (textPtr != IntPtr.Zero)
                    Marshal.FreeHGlobal(textPtr);
            }
        }
    }

    public void SapiOutputFile(string filename, string text, bool xml = false)
    {
        if (IsWindows)
        {
            Sapi_Output_File_Windows(filename, text, xml);
        }
        else
        {
            IntPtr textPtr = StringToWChar(text);
            try
            {
                Sapi_Output_File_Unix(filename, textPtr, xml);
            }
            finally
            {
                if (textPtr != IntPtr.Zero)
                    Marshal.FreeHGlobal(textPtr);
            }
        }
    }

    public void SapiPause() => Sapi_Pause();

    public void SapiResume() => Sapi_Resume();

    public void SapiStop() => Sapi_Stop();
}