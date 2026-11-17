@echo off
setlocal
cd /d "%~dp0"
call "%ProgramFiles%\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat" >nul || exit /b 1
if not exist build\build.ninja ( cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release || exit /b 1 )
cmake --build build -j 4 || exit /b 1
echo [OK] build\PayloadInterpreter.dll
