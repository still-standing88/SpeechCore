public class SpeechCore implements AutoCloseable {
    static {
        System.loadLibrary("SpeechCore");
    }

    public static final int SC_SPEECH_FLOW_CONTROL = 1 << 0;
    public static final int SC_SPEECH_PARAMETER_CONTROL = 1 << 1;
    public static final int SC_VOICE_CONFIG = 1 << 2;
    public static final int SC_FILE_OUTPUT = 1 << 3;
    public static final int SC_HAS_SPEECH = 1 << 4;
    public static final int SC_HAS_BRAILLE = 1 << 5;
    public static final int SC_HAS_SPEECH_STATE = 1 << 6;
    public static final int SC_SSML_SUPPORT = 1 << 7;

    private native void Speech_Init();
    private native void Speech_Free();
    private native void Speech_Detect_Driver();
    private native String Speech_Current_Driver();
    private native String Speech_Get_Driver(int index);
    private native void Speech_Set_Driver(int index);
    private native int Speech_Get_Drivers();
    private native int Speech_Get_Flags();
    private native boolean Speech_Is_Loaded();
    private native boolean Speech_Is_Speaking();
    private native boolean Speech_Output(String text, boolean interrupt);
    private native boolean Speech_Output_Text(String text, boolean interrupt, boolean with_ssml);
    private native boolean Speech_Braille(String text);
    private native boolean Speech_Stop();
    private native float Speech_Get_Volume();
    private native void Speech_Set_Volume(float offset);
    private native float Speech_Get_Rate();
    private native void Speech_Set_Rate(float offset);
    private native float Speech_Get_Pitch();
    private native void Speech_Set_Pitch(float pitch);
    private native String Speech_Get_Current_Voice();
    private native String Speech_Get_Voice(int index);
    private native void Speech_Set_Voice(int index);
    private native int Speech_Get_Voices();
    private native void Speech_Output_File(String filePath, String text);
    private native void Speech_Resume();
    private native void Speech_Pause();

    // SAPI-specific methods (Windows only)
    private native void Speech_Prefer_Sapi(boolean prefer_sapi);
    private native boolean Speech_Sapi_Loaded();
    private native void Sapi_Init();
    private native void Sapi_Release();
    private native String Sapi_Get_Current_Voice();
    private native String Sapi_Get_Voice(int index);
    private native void Sapi_Set_Voice(String voice);
    private native void Sapi_Set_Voice_By_Index(int index);
    private native int Sapi_Get_Voices();
    private native float Sapi_Voice_Get_Volume();
    private native void Sapi_Voice_Set_Volume(float volume);
    private native float Sapi_Voice_Get_Rate();
    private native void Sapi_Voice_Set_Rate(float rate);
    private native void Sapi_Speak(String text, boolean interrupt, boolean xml);
    private native void Sapi_Output_File(String filename, String text, boolean xml);
    private native void Sapi_Pause();
    private native void Sapi_Resume();
    private native void Sapi_Stop();

    public SpeechCore() {
        Speech_Init();
    }

    @Override
    public void close() {
        Speech_Free();
    }

    public void detectDriver() {
        Speech_Detect_Driver();
    }

    public String currentDriver() {
        return Speech_Current_Driver();
    }

    public String getDriver(int index) {
        return Speech_Get_Driver(index);
    }

    public void setDriver(int index) {
        Speech_Set_Driver(index);
    }

    public int getDriverCount() {
        return Speech_Get_Drivers();
    }

    public int getSpeechFlags() {
        return Speech_Get_Flags();
    }

    public boolean isLoaded() {
        return Speech_Is_Loaded();
    }

    public boolean isSpeaking() {
        return Speech_Is_Speaking();
    }

    public boolean speak(String text, boolean interrupt) {
        return Speech_Output(text, interrupt);
    }

    public boolean speakWithSSML(String text, boolean interrupt, boolean with_ssml) {
        return Speech_Output_Text(text, interrupt, with_ssml);
    }

    public boolean outputBraille(String text) {
        return Speech_Braille(text);
    }

    public boolean stop() {
        return Speech_Stop();
    }

    public float getVolume() {
        return Speech_Get_Volume();
    }

    public void setVolume(float volume) {
        Speech_Set_Volume(volume);
    }

    public float getRate() {
        return Speech_Get_Rate();
    }

    public void setRate(float rate) {
        Speech_Set_Rate(rate);
    }

    public float getPitch() {
        return Speech_Get_Pitch();
    }

    public void setPitch(float pitch) {
        Speech_Set_Pitch(pitch);
    }

    public String getCurrentVoice() {
        return Speech_Get_Current_Voice();
    }

    public String getVoice(int index) {
        return Speech_Get_Voice(index);
    }

    public void setVoice(int index) {
        Speech_Set_Voice(index);
    }

    public int getVoiceCount() {
        return Speech_Get_Voices();
    }

    public void outputToFile(String filePath, String text) {
        Speech_Output_File(filePath, text);
    }

    public void resume() {
        Speech_Resume();
    }

    public void pause() {
        Speech_Pause();
    }

    public boolean checkSpeechFlags(int flags) {
        return (getSpeechFlags() & flags) != 0;
    }

    // SAPI-specific methods (Windows only)
    public void preferSapi(boolean preferSapi) {
        Speech_Prefer_Sapi(preferSapi);
    }

    public boolean isSapiLoaded() {
        return Speech_Sapi_Loaded();
    }

    public void sapiInit() {
        Sapi_Init();
    }

    public void sapiRelease() {
        Sapi_Release();
    }

    public String sapiGetCurrentVoice() {
        return Sapi_Get_Current_Voice();
    }

    public String sapiGetVoice(int index) {
        return Sapi_Get_Voice(index);
    }

    public void sapiSetVoice(String voice) {
        Sapi_Set_Voice(voice);
    }

    public void sapiSetVoiceByIndex(int index) {
        Sapi_Set_Voice_By_Index(index);
    }

    public int sapiGetVoices() {
        return Sapi_Get_Voices();
    }

    public float sapiGetVolume() {
        return Sapi_Voice_Get_Volume();
    }

    public void sapiSetVolume(float volume) {
        Sapi_Voice_Set_Volume(volume);
    }

    public float sapiGetRate() {
        return Sapi_Voice_Get_Rate();
    }

    public void sapiSetRate(float rate) {
        Sapi_Voice_Set_Rate(rate);
    }

    public void sapiSpeak(String text, boolean interrupt, boolean xml) {
        Sapi_Speak(text, interrupt, xml);
    }

    public void sapiOutputFile(String filename, String text, boolean xml) {
        Sapi_Output_File(filename, text, xml);
    }

    public void sapiPause() {
        Sapi_Pause();
    }

    public void sapiResume() {
        Sapi_Resume();
    }

    public void sapiStop() {
        Sapi_Stop();
    }
}