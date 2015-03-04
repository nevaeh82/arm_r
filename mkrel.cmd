@echo off

if "%PROCESSOR_ARCHITECTURE%" == "x86" ( call "c:\Program Files\Microsoft Visual Studio 10.0\vc\bin\vcvars32.bat" \
) else ( call "c:\Program Files (x86)\Microsoft Visual Studio 10.0\vc\bin\vcvars32.bat" )

cmake -E remove_directory build
cmake -E make_directory  build

cd build

cmake -G "NMake Makefiles" .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=tmprel -DVERSION=%1

jom all
ctest -V -T Test
cpack

exit /B 0