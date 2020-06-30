@echo off
:: Copyright (c) 2019 The Chromium Embedded Framework Authors. All rights
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

if "%JAVA_HOME%" == "" (
echo ERROR: Please set the JAVA_HOME environment variable
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

:: Remove the first two params (%1 and %2) and pass the rest to java.
set RESTVAR=
shift
shift
:loop1
if "%1"=="" goto after_loop
set RESTVAR=%RESTVAR% %1
shift
goto loop1
:after_loop

:: JUnit can fail to load JVM DLLs if you don't explicitly set the PATH.
set PATH="%JAVA_HOME%\bin"

java -Djava.library.path=%LIB_PATH% -jar .\third_party\junit\junit-platform-console-standalone-1.4.2.jar -cp %OUT_PATH% --disable-ansi-colors --select-package tests.junittests %RESTVAR%

:end
endlocal & set RETURNCODE=%ERRORLEVEL%
goto omega

:returncode
exit /B %RETURNCODE%

:omega
call :returncode %RETURNCODE%
