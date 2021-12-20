@echo off
:: Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
:: reserved. Use of this source code is governed by a BSD-style license
:: that can be found in the LICENSE file.

set RETURNCODE=
setlocal

if "%1" == "" (
echo ERROR: Please specify a build target: win32 or win64
set ERRORLEVEL=1
goto end
)
cd ..\out\%1
jar -cmf manifest\MANIFEST.MF jcef.jar org/cef/*.class org/cef/browser/*.class org/cef/callback/*.class org/cef/handler/*.class org/cef/misc/*.class  org/cef/network/*.class
jar -cf jcef-tests.jar tests/detailed/*.class tests/detailed/dialog/*.class tests/detailed/handler/* tests/detailed/ui/*.class

:end
endlocal & set RETURNCODE=%ERRORLEVEL%
goto omega

:returncode
exit /B %RETURNCODE%

:omega
call :returncode %RETURNCODE%
