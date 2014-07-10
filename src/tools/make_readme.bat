@echo off
:: Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
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

set DISTRIB_TARGET="%1"
set DISTRIB_PATH=".\binary_distrib\%1"

if not exist %DISTRIB_PATH% mkdir %DISTRIB_PATH%

:: Create README.txt
call python.bat tools\make_readme.py --output-dir %DISTRIB_PATH%\ --target %DISTRIB_TARGET%

:end
endlocal & set RC=%ERRORLEVEL%
goto omega

:returncode
exit /B %RC%

:omega
call :returncode %RC%
