@echo off
echo Build ConfigParser documentation
set /p choice = "Install documentation requirements? (y/n)"
if /i "%choice%"=="y" (pip install requirements.txt) else (
echo Skipped
)
doxygen
cd source
sphinx-build -b html . _build