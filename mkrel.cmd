@echo off

pushd 
cd /d %~dp0

if "%PRODUCT_PLATFORM%"=="" (
	echo Error: Platform not defined!
	exit /B 1
)

call %PRODUCT_PLATFORM%\activate.cmd msvc

set MAKE_COMMAND=mingw32-make -r

qtx make clean

echo start build msvc

qtx make msvc-release
if errorlevel 1 exit /B 1 

echo end build msvc


qtx make mkrel


popd
exit /B 0 