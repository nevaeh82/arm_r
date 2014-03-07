@echo off

pushd 
cd /d %~dp0

if "%PRODUCT_PLATFORM%"=="" (
	echo Error: Platform not defined!
	exit /B 1
)

call %PRODUCT_PLATFORM%\activate.cmd msvc

set MAKE_COMMAND=mingw32-make -r

qtx make clean msvc-release mkrel

if errorlevel 1 exit /B 1 

popd
exit /B 0 