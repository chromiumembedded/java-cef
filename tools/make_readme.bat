@echo off
:: Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
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

set DISTRIB_PATH=".\binary_distrib\%1"
if not exist %DISTRIB_PATH% mkdir %DISTRIB_PATH%

:: Create README.txt
call python tools\make_readme.py --output-dir %DISTRIB_PATH%\ --platform %1

:end
endlocal & set RETURNCODE=%ERRORLEVEL%
goto omega

:returncode
exit /B %RETURNCODE%

:omega
call :returncode %RETURNCODE%
