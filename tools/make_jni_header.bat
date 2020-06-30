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
echo ERROR: Please specify a class name
set ERRORLEVEL=1
goto end
)

set OUT_PATH=".\native"
set CLS_PATH=".\third_party\jogamp\jar\*;.\out\%1"
set CLS_NAME=""

SET TMP="%2"
:loop
for /F "tokens=1,* delims=." %%F in (%TMP%) DO (
  set CLS_NAME="%%F"
  set TMP="%%G"
  goto loop
)

call javah.exe -force -classpath %CLS_PATH% -o %OUT_PATH%/%CLS_NAME%.h %2

:end
endlocal & set RETURNCODE=%ERRORLEVEL%
goto omega

:returncode
exit /B %RETURNCODE%

:omega
call :returncode %RETURNCODE%
