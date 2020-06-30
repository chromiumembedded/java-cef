@echo off
:: Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
:: reserved. Use of this source code is governed by a BSD-style license
:: that can be found in the LICENSE file.

set RETURNCODE=
setlocal

cd ..

if "%1" == "" (
echo ERROR: Please specify a target platform: win32 or win64
set ERRORLEVEL=1
goto end
)

if "%2" == "" (
echo ERROR: Please specify a build type: Debug or Release
set ERRORLEVEL=1
goto end
)

if "%3" == "" (
echo ERROR: Please specify a run type: detailed or simple
set ERRORLEVEL=1
goto end
)

set OUT_PATH=.\out\%~1

set LIB_PATH=.\jcef_build\native\%~2
if not exist %LIB_PATH% (
echo ERROR: Native build output path does not exist
goto end
)

set CLS_PATH=.\third_party\jogamp\jar\*;%OUT_PATH%
set RUN_TYPE=%~3

:: Remove the first three params (%1, %2 and %3) and pass the rest to java.
set RESTVAR=
shift
shift
shift
:loop1
if "%1"=="" goto after_loop
set RESTVAR=%RESTVAR% %1
shift
goto loop1
:after_loop

java -cp %CLS_PATH% -Djava.library.path=%LIB_PATH% tests.%RUN_TYPE%.MainFrame %RESTVAR%

:end
endlocal & set RETURNCODE=%ERRORLEVEL%
goto omega

:returncode
exit /B %RETURNCODE%

:omega
call :returncode %RETURNCODE%
