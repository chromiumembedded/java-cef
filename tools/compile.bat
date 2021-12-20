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

set OUT_PATH=".\out\%1"
set CLS_PATH=".\third_party\jogamp\jar\*;.\third_party\junit\*;.\java"

if not exist %OUT_PATH% mkdir %OUT_PATH%
javac -Xdiags:verbose -cp %CLS_PATH% -d %OUT_PATH% java/tests/detailed/*.java java/tests/junittests/*.java java/tests/simple/*.java java/org/cef/*.java java/org/cef/browser/*.java java/org/cef/callback/*.java java/org/cef/handler/*.java java/org/cef/misc/*.java java/org/cef/network/*.java

:: Copy MANIFEST.MF
xcopy /sfy .\java\manifest %OUT_PATH%\manifest\

:: Copy resource files.
xcopy /sfy .\java\tests\detailed\handler\*.html %OUT_PATH%\tests\detailed\handler\
xcopy /sfy .\java\tests\detailed\handler\*.png %OUT_PATH%\tests\detailed\handler\

:end
endlocal & set RETURNCODE=%ERRORLEVEL%
goto omega

:returncode
exit /B %RETURNCODE%

:omega
call :returncode %RETURNCODE%
