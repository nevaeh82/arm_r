@echo off

rem init environment vars for Visual C
set VCVARS_BAT="c:\Program Files (x86)\Microsoft Visual Studio 10.0\vc\bin\vcvars32.bat"
if not exist %VCVARS_BAT% ( set VCVARS_BAT="c:\Program Files\Microsoft Visual Studio 10.0\vc\bin\vcvars32.bat" )
call %VCVARS_BAT%
if not errorlevel 0 goto error

call build.cmd release %1
if not errorlevel 0 goto error

goto no_error

:error
exit /B %errorlevel%

:no_error
exit /B 0