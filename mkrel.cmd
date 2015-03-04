@echo off

rem init environment vars for Visual C
set VCVARS_BAT="c:\Program Files (x86)\Microsoft Visual Studio 10.0\vc\bin\vcvars32.bat"
if not exist %VCVARS_BAT% ( set VCVARS_BAT="c:\Program Files\Microsoft Visual Studio 10.0\vc\bin\vcvars32.bat" )
call %VCVARS_BAT%
if not errorlevel 0 goto error

cmake -E remove_directory build
if not errorlevel 0 goto error

cmake -E make_directory  build
if not errorlevel 0 goto error

cd build
if not errorlevel 0 goto error

cmake -G "NMake Makefiles" .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=tmprel -DVERSION=%1
if not errorlevel 0 goto error2

jom all
if not errorlevel 0 goto error2

ctest -V -T Test
if not errorlevel 0 goto error2

cpack
if not errorlevel 0 goto error2

goto no_error

:error2
set last_error=%errorlevel%
cd ..
set errorlevel=%last_error%

:error
exit /B %errorlevel%

:no_error
cd ..
exit /B 0