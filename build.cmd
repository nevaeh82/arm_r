@echo off

rem arg1 - build type
rem arg2 - version
set build_type=%1
set version=%2

rem prepare build type
if "%build_type%" == "" set build_type=debug
if "%build_type%" == "Debug" set build_type=debug
if "%build_type%" == "Release" set build_type=release

rem clean build folder
cmake -E remove_directory build
if not errorlevel 0 goto error

cmake -E make_directory build
if not errorlevel 0 goto error

cd build
if not errorlevel 0 goto error

rem configure build
cmake -G "NMake Makefiles" .. -DCMAKE_BUILD_TYPE=%build_type% -DCMAKE_INSTALL_PREFIX=tmprel -DVERSION=%version%
if not errorlevel 0 goto error2

rem run build
jom all
if not errorlevel 0 goto error2

rem run tests
ctest -V -T Test
if not errorlevel 0 goto error2

rem do cpack only for release
if not "%build_type%" == "release" goto no_error

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