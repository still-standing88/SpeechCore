# SpeechCore

SpeechCore is a cross-platform C++ library that abstracts the process of communicating with various screen readers, managing low-level details and providing a clean, simple-to-use interface.

## Features

* Simple and intuitive API with straightforward usage
* Cross-platform support for various screen readers, with the option to support additional ones

## Installation

1. Download the matching library build for your platform from [the releases page](https://github.com/still-standing88/SpeechCore/releases).
2. Link against the library (static or shared, depending on your choice).
3. Copy the contents of the include folder to your project directory.
4. Include `SpeechCore.h` in your project.

## Building

The GitHub repo includes a Visual Studio solution and a SConstruct file. If you're using a different build system, keep the following notes in mind:

* Define either `__SPEECH_C_EXPORT` (shared) or `SPEECH_C_STATIC` (static) when compiling.
* JNI files are included for Java support. Ensure you have the required Java files, or exclude them if not needed.
* Include only platform-specific files for your target platform.
* Linux builds require the Speech Dispatcher library.
* Windows builds need to link against SAPI.LIB.
* Macos builds need to link against object library. And the AVFoundation and Foundation frameworks.
* Documentation generation requires Doxygen and Sphinx.

## Usage

Simple usage example:

```cpp
#include <iostream>
#include <SpeechCore.h>

int main() {
    Speech_Init();
    if (Speech_Is_Loaded()) {
        std::cout << "Current screen reader: " << Speech_Get_Current_Driver() << std::endl;
        std::cout << "Speaking some text" << std::endl;
        Speech_Output(L"This is a test for the SpeechCore library. If you're hearing this, it indicates the library is functioning properly.");
    }

    Speech_Free(); // Free resources when you're done.
    return 0;
}
```

See the documentation for more detailed usage examples.


# Documentation

Documentation can be found [here](https://still-standing88.github.io/SpeechCoreDocs/).

## Library Notes

* Windows screen readers (NVDA, JAWS, Zhengdu, PCTalker, and System Access) require specific binaries, included with the source code.
* Enhanced control over SAPI 5, including voice configuration and speech parameters.
* Braille output functionality is included and implemented for the screen readers that support it. Mainly NVDA and Jaws

## Supported Screen Readers

* Windows: NVDA, JAWS, System Access, Zhengdu Screen Reader, PCTalker, SAPI 5
* macOS: AVSpeech
* Linux: Speech Dispatcher

## Bindings

Bindings are available for Python, .NET/C#, and Java.

For Python
Install python bindings via pip:
```bash
pip install SpeechCore
```

Dotnet
Install via NuGet Package Manager:
```bash
dotnet add package SpeechCore.CrossPlatform
```
Or via Package Manager Console in Visual Studio:
```bash
Install-Package SpeechCore.CrossPlatform
```

## Credits

This library was inspired by [Tolk](https://github.com/dkager/tolk/), with adaptations for more flexibility. It was initially developed for personal projects and later expanded to be cross-platform.

## Change Log

### Version 1.1.1
* The Speech_Detect_Driver now rescans for drivers on Windows even if one is currently running. 

### Version 1.1.0
* Fixed NVDA speech interrupt functionality.
* New functions available on api:
  * `Speech_Set_Pitch` and `Speech_Get_Pitch`. For controling pitch parameter for drivers that support it.
  * `Speech_Output_Text`. An enhanced speech function which includes an with_ssml parameter to use if the speech driver supports it. The SC_SSML_SUPPORT an be used to check if it does.

### Version 1.0.2
* Added PCTalker screen reader support.
* Implemented braille functionality for screen readers that support it (NVDA, jaws)
* Modified Encoding handling for Unix platforms, it should function properly now.
* Python bindings have been rewritten with pybind11 an is now a a part of the main repo.
* Dotnet bindings are now available as a nuget package.

## Contributions

Contributions to the library or support for additional screen readers are welcome.

## License

[MIT License](LICENSE)

## Todo List
- [ ] Implement Android/iOS support
- [ ] Add Meson build files
- [ ] Add CMake build files