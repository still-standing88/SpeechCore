import platform as pf
import sys
import os
import glob
import shutil

import pybind11

from setuptools import Extension, setup, find_packages
from setuptools.command.build_ext import build_ext
from setuptools.command.install import install
from setuptools.command.sdist import sdist
from wheel.bdist_wheel import bdist_wheel

def get_readme():
    try:
        with open("./python_readme.md", 'r', encoding='utf-8') as file:
            return file.read()
    except FileNotFoundError:
        return "SpeechCore Python Wrapper (pybind11)"

def detect_platform():
    platform = sys.platform
    if platform == 'win32':
        return 'windows'
    elif platform == 'darwin':
        return 'macos'
    elif platform == 'linux':
        return 'linux'
    else:
        print(f'Unknown platform {platform}. Defaulting to windows.')
        return 'windows'

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

def find_static_lib(build_type_hint=None):
    platform = detect_platform()
    arch = detect_host_arch()
    
    build_types = []
    if build_type_hint and build_type_hint in ['release', 'debug']:
        build_types.append(build_type_hint)
    build_types.extend(['release', 'debug'])
    dir_names = ['static']

    for build_attempt in list(dict.fromkeys(build_types)):
        for subdir in dir_names:
            lib_dir_base = os.path.join('lib', platform, arch, build_attempt, subdir)
            
            if platform == 'windows':
                lib_file_path = os.path.join(lib_dir_base, 'SpeechCore.lib')
            else:
                lib_file_path = os.path.join(lib_dir_base, 'libSpeechCore.a')
            
            if os.path.exists(lib_file_path):
                return lib_dir_base, lib_file_path, build_attempt
    
    raise FileNotFoundError(
        f"SETUP.PY_ERROR: Could not find SpeechCore static library for Python in expected paths for "
        f"platform {platform}, arch {arch}"
    )

platform = detect_platform()
arch = detect_host_arch()
build_type = os.environ.get('PYTHON_BUILD_TYPE') 
sources = ["./speech_core/bindings.cpp"] 
extra_compile_args = ["-D__SPEECH_C_STATIC"] 
extra_link_args = []
link_libraries = ['SpeechCore']
library_dirs = []

try:
    found_lib, lib_path, lib_build_type = find_static_lib(build_type)
except FileNotFoundError as e:
    print(f"SETUP.PY_ERROR: {e}")

library_dirs += [os.path.abspath(found_lib)]

if platform == 'windows':
    crt_flag = "/MTd" if build_type == 'debug' else '/MT'
    extra_compile_args += [crt_flag, '/std:c++20', '/EHsc', '/D_WIN32', "/DUNICODE", "/D_UNICODE", "/W3"]
    extra_link_args.extend(["User32.lib", "Ole32.lib"]) 
elif platform == "macos":
    extra_compile_args.extend(['-std=c++20', "-fvisibility=hidden", "-Wall", "-Wextra", "-DMACOS", "-D__MACOSX_CORE__", "-mmacosx-version-min=10.15"])
    extra_link_args.extend(["-framework", "CoreFoundation", "-framework", "ApplicationServices"])
else:
    extra_compile_args += ['-std=c++2a', "-fvisibility=hidden", "-Wall", "-Wextra", "-DLINUX"]

ext_modules = [
    Extension(
        name="speech_core.SpeechCore",
        sources=sources,
        include_dirs=[
            os.path.abspath("./include/"), 
            pybind11.get_include(),
        ],
        library_dirs=library_dirs,
        libraries=link_libraries,
        extra_compile_args=extra_compile_args,
        extra_link_args=extra_link_args,
        language="c++"
    )
]

class dllDist(sdist):

    def run(self):
        super().run()
        if platform == 'windows':
            print("Copying dll files")
            dll_dir = f"bin/{'x64' if '64' in arch else 'x86'}"
            dll_target_dir = "speech_core"
            if os.path.exists(dll_dir):
                for file in os.listdir(dll_dir):
                    if file.endswith('.dll'):
                        source = os.path.join(dll_dir, file)
                        target = os.path.join(dll_target_dir, file)
                        print(f'Copying file: {source}. To {target}')
                        self.copy_file(source, target)

class postCleanup(bdist_wheel):

    def run(self):
        super().run()
        if platform == 'windows':
            print('Cleaning dll files.')
            for file in os.listdir('./speech_core'):
                if file.endswith('.dll'):
                    os.remove(f'./speech_core/{file}')

setup(
    name="SpeechCore",
    version="1.1.1",
    author="still-standing88",
    url="https://github.com/still-standing88/speech-core/",
    description="Python wrapper for the SpeechCore library (pybind11)",
    long_description=get_readme(),
    long_description_content_type="text/markdown",
    license="MIT", 
    packages=find_packages(where=".", include=["speech_core*"]),
    ext_modules=ext_modules,
    package_data={'speech_core': ['*.dll']},
    cmdclass={'build_ext': build_ext,
    'sdist': dllDist,
    'bdist_wheel': postCleanup},
    classifiers=[
        "Programming Language :: Python :: 3",
        "Programming Language :: Python :: 3.8",
        "Programming Language :: Python :: 3.9",
        "Programming Language :: Python :: 3.10",
        "Programming Language :: Python :: 3.11",
        "Programming Language :: Python :: 3.12",
        "Programming Language :: Python :: 3.13",
        "License :: OSI Approved :: MIT License",
        "Operating System :: Microsoft :: Windows",
        "Operating System :: MacOS :: MacOS X",
        "Operating System :: POSIX :: Linux",
    ],
    python_requires=">=3.8",
    setup_requires=['pybind11>=2.6'], 
    install_requires=[],
    zip_safe=False,
)
