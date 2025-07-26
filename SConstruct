# SConstruct file for building SpeechCore.
# Some paths here for Linux/macos have bin edited for GitHub actions use.
# Compilers for platform by default are:
# MSVC on Windows, GCC on Linux, and Clang on Mac.
# It is recommended to use the compilers for each platform as specified above.


import platform as pf
import os
import sys
import shutil

from SCons.Script import Environment, DefaultEnvironment, Import, Exit
from SCons.Script import Variables, BoolVariable, EnumVariable, ARGUMENTS

# Functions for handling build process.

def build_python_wheel(target, source, env):
    global platform, target_arch, build_type    

    print("Building Python wheel...")
    os.environ['PYTHON_BUILD_TYPE'] = build_type    

    try:
        cmd = f"{sys.executable} setup.py sdist bdist_wheel"
        result = os.system(cmd)
        if result != 0:
            print("ERROR: Python wheel build failed")
            return 1
        
        print("Python wheel built successfully")
        return 0
    except Exception as e:
        print(f"ERROR building Python wheel: {str(e)}")
        return 1

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

def detect_host_arch():
    machine = pf.machine().lower()
    if machine in ['x86_64', 'amd64']:
        return 'x86_64'
    elif machine in ['i386', 'i686']:
        return 'x86'
    elif machine in ['arm64', 'aarch64']:
        return 'arm64'
    else:
        print(f"Warning: Unrecognized architecture '{machine}'. Defaulting to x86_64.")
        return 'x86_64'


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

# Handling of rc resource files on windows.
def compile_rc():
    global rc_files
    if platform == 'windows' and build_mode== 'shared':
        print(rc_files)
        print('compiling rc files')
        for file in rc_files:
            cmd = f'{rc_path} {file}'
            if os.system(cmd) !=0:
                print(f'Error compiling {file}')
                return 1
        return 0
    return 0


def build_cleanup(target, source, env):
    global resource_files
    try:
        if cleanup == True and os.path.exists('./build'):
            shutil.rmtree('build')
        for file in resource_files:
            if os.path.exists(file):
                os.remove(file)
    except Exception as e:
        print(str(e))
        return 1
    return 0

# Build variables
#Edit if Sapi path is different in your machine (Windows only)
#this is architecture specific 
sapi_path = {'x86_64': 'C:/Program Files (x86)/Windows Kits/10/Lib/10.0.22621.0/um/x64/',
    'x86': 'C:/Program Files (x86)/Windows Kits/10/Lib/10.0.22621.0/um/x86/'
}

# Rc file resource compilor
#edit if for full path if not using Visual Studio developer cmd prompt.
rc_path = 'rc.exe'

# Macos Java paths, configured for GitHub actions.
java_macos_include_dir = '/Users/runner/hostedtoolcache/Java_Zulu_jdk/21.0.4-7/arm64/include'
java_macos_lib_dir = '/Users/runner/hostedtoolcache/Java_Zulu_jdk/21.0.4-7/arm64/lib/server'

host_arch = detect_host_arch()
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

vars.Add(EnumVariable(
    'arch',
    'Choose target architecture',
    host_arch,
    allowed_values=('x86_64', 'arm64', 'x86')
))

vars.Add('compiler', 'Specify the compiler to use', 'default')
vars.Add(BoolVariable(
    'with_java',
    'Enable or disable Java support',
    True
))

vars.Add(BoolVariable(
    'cleanup',
    'Delete files from the build process.',
    False
))

vars.Add(BoolVariable(
    'build_python',
    'Build Python wheel package',
    False
))

res_file = None
_cli_arch = ARGUMENTS.get('arch', host_arch)
_msvc_arch = None

if sys.platform.startswith('win'):
    if _cli_arch== 'x86': _msvc_arch = 'x86'
    elif _cli_arch== 'x86_64': _msvc_arch = 'amd64'
env = Environment(variables=vars, TARGET_ARCH=_msvc_arch if _msvc_arch else None)

vars.Update(env)


# Main script starts here
platform = detect_platform(env)
target_arch = env.get('arch')
compiler = detect_compiler(env)
java_support = env.get('with_java', True)
cleanup = env.get('cleanup', False)
build_python = env.get('build_python', False)
java_home = ""

print("Detected platform: {}".format(platform))
print(f"Building for architecture: {env['arch']}")
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
    # Add the following flags since clang keaps complaining about extern variables with initialized values and unused parameters.
    '-Wno-extern-initializer', '-Wno-unused-parameter', '-Wno-deprecated-declarations'])
    if platform == 'macos':
        env.Append(CXXFLAGS=['-isysroot', '/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk'])
elif compiler == 'gcc':
    env.Replace(CC='gcc')
    env.Replace(CXX='g++')
    env.Append(CXXFLAGS=['-std=c++20', '-Wall', '-Wextra', '-Wno-unused-parameter'])
else:
    env.Append(CXXFLAGS=['-std=c++20'])

# Handle build type (debug/release)
build_type = env.get('build_type', 'release')
if build_type == 'debug':
    if compiler == 'msvc':
        env.Append(CCFLAGS=['/Zi', '/MTd'])
        env.Append(LINKFLAGS=['/DEBUG'])
    else:
        env.Append(CXXFLAGS=['-g'])
    env.Append(CCFLAGS=['-DDEBUG', ])
else:
    if compiler == 'msvc':
        env.Append(CCFLAGS=['/O2', '/MT'])
    else:
        env.Append(CXXFLAGS=['-O2'])
    env.Append(CCFLAGS=['-DNDEBUG'])

# Handle build mode (static/shared)
build_mode = env.get('build_mode', 'shared')
if build_mode == 'static':
    env.Append(CPPDEFINES=['__SPEECH_C_STATIC'])
else:
    env.Append(CPPDEFINES='__SPEECH_C_EXPORT')
    if platform == 'windows' and compiler == 'msvc':
        env.Append(LINKFLAGS='/DLL')
    elif platform == 'linux' or platform == 'macos':
        env.Append(CXXFLAGS=['-fPIC'])

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
    if '64' in target_arch:
        env.Append(CPPDEFINES=['_WIN64'])
    env.Append(CPPDEFINES=['WIN32'])
    env.Append(LIBS=['sapi', 'ole32', 'user32', 'oleaut32'])
    env.Append(LIBPATH=[sapi_path[target_arch]])
elif platform == 'macos':
    env.Append(CPPDEFINES=['MACOS'])
    env.Append(FRAMEWORKS=['Foundation', 'AVFoundation'])
    env.Append(LIBS=['objc'])
    env.Append(OBJCFLAGS=['-fobjc-arc'])
    env['OBJCXXCOM'] = env['CXXCOM']
    env.Append(CCFLAGS=['-isysroot', '/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk'])
    env.Append(CPPPATH=[java_macos_include_dir,
        f'{java_macos_include_dir}/darwin'])
    env.Append(LINKFLAGS=[f'-L{java_macos_lib_dir}'])
elif platform == 'linux':
    env.Append(CPPDEFINES=['LINUX'])
    env.Append(LIBS=['speechd'])

# Platform arch related flags
if platform == 'windows':
    if target_arch == 'x86_64':
        env.Append(LINKFLAGS=['/MACHINE:X64'])
    elif target_arch == 'x86':
        env.Append(LINKFLAGS=['/MACHINE:X86', '/SAFESEH:NO'])
elif platform == 'macos':
    env.Append(CCFLAGS=['-arch', 'x86_64', '-arch', 'arm64'])
    env.Append(LINKFLAGS=['-arch', 'x86_64', '-arch', 'arm64'])
    env.Append(CCFLAGS=['-mmacosx-version-min=10.14'])
    #env.Append(LINKFLAGS=['-mmacosx-version-min= 10.14'])

elif platform == 'linux':
    env.Append(CXXFLAGS=['-fPIC'])
    if target_arch == 'x86_64':
        env.Append(CCFLAGS=['-march=x86-64'])
    elif target_arch == 'arm64':
        env.Append(CCFLAGS=['-march=armv8-a'])


# Set up build directories
build_dir = 'build'
lib_dir = f'lib/{platform}/{env["arch"]}/{build_type}/{build_mode}/'
if not os.path.exists(build_dir):
    os.makedirs(build_dir)
if not os.path.exists(lib_dir):
    os.makedirs(lib_dir)

# Set up include paths. 
env.Append(CPPPATH=[os.path.join('include')])
src_dir = 'src'
src_files = []
rc_files = [f'{src_dir}/SpeechCore.rc',]
resource_files = []

# Platform-specific exclusions. If any new screen readers specific to a platform end up being here, this list has to be updated.
windows_exclude = ['sapi5driver.cpp', 'SapiSpeech.cpp', 'nvda.cpp', 'jaws.cpp', 'sa.cpp', 'pc_talker.cpp', 'zdsr.cpp', 'zdsrapi.cpp', 'saapi.cpp', 'fsapi.c', 'wasapi.cpp']
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


if platform == 'windows' and build_mode== 'shared':
    resource_files += [file.replace('.rc', '.res') for file in rc_files]

env.VariantDir(build_dir, src_dir, duplicate=0)
compile_rc()


# Build the library
if build_mode == 'static':
    lib = env.StaticLibrary(os.path.join(lib_dir, 'SpeechCore'), src_files)
else:
    lib = env.SharedLibrary(os.path.join(lib_dir, 'SpeechCore'), src_files+resource_files)
env.AddPostAction(lib, build_cleanup)

# Build python package
if build_python:
    if build_mode != 'static':
        print("WARNING: Python wheel build requires static library. Skipping python package build.")
    else:
        python_wheel = env.Command('python_wheel_built', lib, build_python_wheel)
        env.Depends(python_wheel, lib)


# Set correct library prefix and suffix based on platform
if platform == 'windows':
    pass  # SCons handles this automatically
elif platform == 'macos':
    env.Replace(SHLIBPREFIX='lib')
    env.Replace(SHLIBSUFFIX='.dylib')
elif platform == 'linux':
    env.Replace(SHLIBPREFIX='lib')
    env.Replace(SHLIBSUFFIX='.so')

if build_python:
    Default([lib, python_wheel])
else:
    Default(lib)