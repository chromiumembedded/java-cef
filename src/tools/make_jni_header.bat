@echo off
:: Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
:: reserved. Use of this source code is governed by a BSD-style license
:: that can be found in the LICENSE file.

set RC=
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

call javah.exe -force -classpath %CLS_PATH% -d %OUT_PATH% org.cef.%2
del /F %OUT_PATH%\%2.h
rename %OUT_PATH%\org_cef_%2.h %2.h

:end
endlocal & set RC=%ERRORLEVEL%
goto omega

:returncode
exit /B %RC%

:omega
call :returncode %RC%
