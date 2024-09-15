 
import os
import sys
from SCons.Script import Environment, DefaultEnvironment, Import, Exit
from SCons.Script import Variables, BoolVariable, EnumVariable, ARGUMENTS

#Edit if Sapi path is different in your machine (Windows only)
sapi_path = 'C:/Program Files (x86)/Windows Kits/10/Lib/10.0.22621.0/um/x64/'

vars = Variables()

vars.Add(EnumVariable(
    'build_mode',
    'Choose build mode (static/shared)',
    'shared',
    allowed_values=('static', 'shared')
))

vars.Add(EnumVariable(
    'build_type',
    'Choose build type (debug/release)',
    'release',
    allowed_values=('debug', 'release')
))

vars.Add('compiler', 'Specify the compiler to use', 'default')
vars.Add(BoolVariable(
    'with_java',
    'Enable or disable Java support',
    True
))

env = Environment(variables=vars)
vars.Update(env)
res_file = None

def handle_java(env):
    env.Append(CPPDEFINES=['__WITH_JAVA'])
    if platform == 'windows':
        java_include = os.path.join(java_home, 'include')
        java_win_include = os.path.join(java_include, 'win32')
        env.Append(CPPPATH=[java_include, java_win_include])
        jvm_lib = os.path.join(java_home, 'lib', 'jvm.lib')
        env.Append(LIBS=[jvm_lib])
    elif platform == 'macos':
        java_include = os.path.join(java_home, 'include')
        java_mac_include = os.path.join(java_include, 'darwin')
        env.Append(CPPPATH=[java_include, java_mac_include])
        jvm_lib_path = os.path.join(java_home, 'lib', 'server', 'libjvm.dylib')
        if os.path.exists(jvm_lib_path):
            env.Append(LIBPATH=[os.path.dirname(jvm_lib_path)])
            env.Append(LIBS=[os.path.basename(jvm_lib_path)])
        else:
            print(f"ERROR: JVM library not found at {jvm_lib_path}. Disabling java support")
            java_support = False
        env.Append(LIBS=['libjvm'])
    elif platform == 'linux':
        java_include = os.path.join(java_home, 'include')
        java_linux_include = os.path.join(java_include, 'linux')
        env.Append(CPPPATH=[java_include, java_linux_include])
        env.Append(LIBPATH=[os.path.join(java_home, 'lib/server'), os.path.join(java_home, 'lib')])
        env.Append(LIBS=['libjvm'])


def detect_platform(env):
    platform = env['PLATFORM']
    if platform == 'win32':
        return 'windows'
    elif platform == 'darwin':
        return 'macos'
    else:
        return 'linux'

def detect_compiler(env):
    compiler = env.get('compiler', 'default')
    if compiler == 'msvc':
        return 'msvc'
    elif compiler == 'clang':
        return 'clang'
    elif compiler == 'gcc':
        return 'gcc'
    else:
        if env['CC'] == 'cl':
            return 'msvc'
        elif platform == 'macos':
            return 'clang'  # Default to Clang on macOS
        elif 'clang' in env['CC']:
            return 'clang'
        else:
            return 'gcc'

# Main script starts here
platform = detect_platform(env)
compiler = detect_compiler(env)
java_support = env.get('with_java', True)
java_home = ""

print("Detected platform: {}".format(platform))
print("Using compiler: {}".format(compiler))

# Set compiler-specific flags and defines
if compiler == 'msvc':
    env.Replace(CC='cl')
    env.Replace(CXX='cl')
    env.Replace(LINK='link')
    env.Append(CXXFLAGS=['/std:c++20', '/EHsc'])
    env.Append(CPPDEFINES=['_CRT_SECURE_NO_WARNINGS'])
elif compiler == 'clang':
    env.Replace(CC='clang')
    env.Replace(CXX='clang++')
    env.Append(CXXFLAGS=['-std=c++20', '-Wall', '-Wextra',
    # Add thefollowing flags since clang keaps complaining about extern variables with initialized values and unused parameters.
    '-Wno-extern-initializer', '-Wno-unused-parameter', '-Wno-deprecated-declarations'])
    if platform == 'macos':
        env.Append(CXXFLAGS=['-isysroot', '/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk'])
elif compiler == 'gcc':
    env.Replace(CC='gcc')
    env.Replace(CXX='g++')
    env.Append(CXXFLAGS=['-std=c++20', '-Wall', '-Wextra', '-Wno-extern-initializer', '-Wno-unused-parameter'])
else:
    env.Append(CXXFLAGS=['-std=c++20'])

# Handle build type (debug/release)
build_type = env.get('build_type', 'release')
if build_type == 'debug':
    if compiler == 'msvc':
        env.Append(CCFLAGS=['/Zi'])
        env.Append(LINKFLAGS=['/DEBUG'])
    else:
        env.Append(CXXFLAGS=['-g'])
    env.Append(CCFLAGS=['-DDEBUG'])
else:
    if compiler == 'msvc':
        env.Append(CCFLAGS=['/O2'])
    else:
        env.Append(CXXFLAGS=['-O2'])
    env.Append(CCFLAGS=['-DNDEBUG'])

# Handle build mode (static/shared)
build_mode = env.get('build_mode', 'shared')
if build_mode == 'static':
    if platform == 'windows' and compiler == 'msvc':
        env.Append(CPPDEFINES=['__SPEECH_C_STATIC'])
    lib_builder = env.StaticLibrary
else:
    if platform == 'windows' and compiler == 'msvc':
        env.Append(CPPDEFINES='__SPEECH_C_EXPORT')
        env.Append(LINKFLAGS='/DLL')
    elif platform == 'linux' or platform == 'macos':
        env.Append(CXXFLAGS=['-fPIC'])
    lib_builder = env.SharedLibrary

# Handle Java compilation support
if java_support:
    java_home = os.environ.get('JAVA_HOME', None)
    if not java_home:
        print("ERROR: JAVA_HOME is not set. Please set JAVA_HOME to your Java installation path or install  Java if not installed on your system. \rDisabling Java support.")
        java_support  = False
    else:
        handle_java(env)
        print("Java support enabled")

# Platform-specific defines and flags
env.Append(CPPDEFINES=['UNICODE', '_UNICODE'])
if platform == 'windows':
    if '64' in env['TARGET_ARCH']:
        env.Append(CPPDEFINES=['_WIN64'])
    env.Append(CPPDEFINES=['WIN32'])
    env.Append(LIBS=['sapi'])
    env.Append(LIBPATH=[sapi_path])
elif platform == 'macos':
    env.Append(CPPDEFINES=['MACOS'])
    env.Append(FRAMEWORKS=['Foundation', 'AVFoundation'])
    env.Append(LIBS=['objc'])
    env.Append(OBJCFLAGS=['-fobjc-arc'])
    env['OBJCXXCOM'] = env['CXXCOM']
    env.Append(CCFLAGS=['-isysroot', '/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk'])
    env.Append(CPPPATH=['/Users/runner/hostedtoolcache/Java_Zulu_jdk/21.0.4-7/arm64/include',
        '/Users/runner/hostedtoolcache/Java_Zulu_jdk/21.0.4-7/arm64/include/darwin'])
    env.Append(LINKFLAGS=['-L/Users/runner/hostedtoolcache/Java_Zulu_jdk/21.0.4-7/arm64/lib/server'])
elif platform == 'linux':
    env.Append(CPPDEFINES=['LINUX'])
    env.Append(LIBS=['speechd'])


# Set up build directories
build_dir = 'build'
lib_dir = f'lib/{platform}/{build_type}/{build_mode}/'
if not os.path.exists(build_dir):
    os.makedirs(build_dir)
if not os.path.exists(lib_dir):
    os.makedirs(lib_dir)

# Set up include paths. 
env.Append(CPPPATH=[os.path.join('include')])
src_dir = 'src'
src_files = []

# Platform-specific exclusions. If any new screen readers specific to a platform end up being here this list has to be updated.
windows_exclude = ['sapi5driver.cpp', 'SapiSpeech.cpp', 'nvda.cpp', 'jaws.cpp', 'sa.cpp', 'zdsr.cpp', 'zdsrapi.cpp', 'saapi.cpp', 'fsapi.c', 'wasapi.cpp']
exclude_files = {
    'windows': ['SpeechDispatcher.cpp', 'AVSpeech.mm', 'AVTts.cpp'],
    'macos': ['SpeechDispatcher.cpp', *windows_exclude],
    'linux': ['AVTts.cpp', 'AVSpeech.mm', *windows_exclude]
}

for root, dirs, files in os.walk(src_dir):
    for file in files:
        if file.endswith(('.cpp', '.c', '.mm')):
            full_path = os.path.join(root, file)
            if file not in exclude_files[platform]:
                if file.endswith('SpeechCore_JNI.cpp'):
                    continue
                rel_path = os.path.relpath(full_path, src_dir)
                build_path = os.path.join(build_dir, rel_path)
                src_files.append(build_path)

if java_support:
    src_files.append(os.path.join(build_dir, 'SpeechCore_JNI.cpp'))

env.VariantDir(build_dir, src_dir, duplicate=0)

# Build the library
if build_mode == 'static':
    lib = env.StaticLibrary(os.path.join(lib_dir, 'SpeechCore'), src_files)
else:
    lib = env.SharedLibrary(os.path.join(lib_dir, 'SpeechCore'), src_files)

# Set correct library prefix and suffix based on platform
if platform == 'windows':
    pass  # SCons handles this automatically
elif platform == 'macos':
    env.Replace(SHLIBPREFIX='lib')
    env.Replace(SHLIBSUFFIX='.dylib')
elif platform == 'linux':
    env.Replace(SHLIBPREFIX='lib')
    env.Replace(SHLIBSUFFIX='.so')

Default(lib)