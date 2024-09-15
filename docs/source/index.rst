SpeechCore Documentation
========================

.. toctree::
   :maxdepth: 2
   :caption: Contents:

   library_notes
   api

Indices and tables
==================

* :ref:`genindex`
* :ref:`modindex`
* :ref:`search`

SpeechCore
==========

SpeechCore is a cross-platform C++ library that abstracts the process of communicating with various screen readers, managing low-level details and providing a clean, simple-to-use interface. The library has implementations for other languages; see the wrappers section.

Features
--------

* Simple and intuitive API with straightforward usage.
* Cross-platform support for various screen readers, with the option to support additional ones.

Installation
------------

Download the matching library build for your platform from
`the releases page <https://github.com/still-standing88/SpeechCore/releases>`_ and link against the library (static or shared, depending on your choice).
Then copy the contents of the include folder to your project directory and include SpeechCore.h.

Building
--------

The GitHub repo includes a Visual Studio solution and a SConstruct file. If you're using a different build system, keep the following notes in mind:

* To compile for either shared or static, define either ``__SPEECH_C_EXPORT`` (shared) or ``SPEECH_C_STATIC`` (static), depending on your choice.
* The library has JNI-defined .cpp and .h files for the Java interface. Ensure that you have the required Java include files and library available to compile with Java support. You can skip this by excluding the JNI-related files from your build process, as the library does not depend on them.
* Only include platform-related files when building for your target platform to avoid errors. For Windows, this means excluding SpeechDispatcher.cpp, AVTts.cpp, and AVSpeech.cpp from the building process.
* The Speech Dispatcher library is required when compiling for Linux. The library depends on it for Speech Dispatcher support.
* When compiling for Windows, you have to link against SAPI.LIB.
* To generate this documentation, you require Doxygen and Sphinx. The docs folder includes the requirements file for the documentation libraries.

Usage
-----

Simple usage example:

.. code-block:: cpp

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

See the examples folder on the Github repo for more detailed usage.

Library Notes
-------------

* The library requires specific binaries to interface with screen readers on Windows (NVDA, JAWS, Zhengdu, and System Access). You can find the respective binaries included with the source code; they must be included alongside the library in your project.
* The library has more control over SAPI 5 and allows changing of voice configuration and speech parameters. It also has more responsive output.
* The library has braille output functionality that is yet to be implemented. It includes abstract functions which can be implemented to support braille on NVDA, JAWS, and any other screen reader that may support it.

List of supported screen readers
--------------------------------

* Windows: NVDA, JAWS, System Access, Zhengdu Screen Reader, SAPI 5
* macOS: AVSpeech
* Linux: Speech Dispatcher

Wrappers
--------

Wrappers exist for Python, .NET/C#, and Java.

Credits
-------

This library's idea was inspired by Tolk. 
`GitHub page <https://github.com/dkager/tolk/>`_ I adapted a similar structure when building the library but with a little more flexibility.
At the time this library was being built, it was used in personal projects of mine. Since there was no straightforward solution for working with SAPI 5 with full flexibility, I built a wrapper around it. Later, I expanded it to be cross-platform.

Contributions
-------------

Any contributions to the library or support for other screen readers are welcome.
