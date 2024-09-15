@echo off
set "build_mode=shared"
set "build_type=release"

set /p choice="Change default build options (Y/N): "
if /i "%choice%"=="y" (
    goto options
) else (
    echo Using default options...
    goto build
)

:options
set /p option1="Build mode (1 for static, 2 for shared): "
if "%option1%"=="1" (
    set "build_mode=static"
) else if "%option1%"=="2" (
    set "build_mode=shared"
) else (
    echo Invalid option for build mode. Using default.
)

set /p option2="Build type (1 for debug, 2 for release): "
if "%option2%"=="1" (
    set "build_type=debug"
) else if "%option2%"=="2" (
    set "build_type=release"
) else (
    echo Invalid option for build type. Using default.
)

:build
echo Build mode is %build_mode%
echo Build type is %build_type%

scons build_mode="%build_mode%" build_type="%build_type%"
