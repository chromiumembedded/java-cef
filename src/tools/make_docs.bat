@echo off
:: Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
:: reserved. Use of this source code is governed by a BSD-style license
:: that can be found in the LICENSE file.

set RC=
setlocal
cd ..\java

set OUT_PATH="..\out\docs"

if not exist %OUT_PATH% mkdir %OUT_PATH%
javadoc -windowtitle "CEF3 Java API Docs" -footer "<center><a href="http://code.google.com/p/javachromiumembedded" target="_top">Chromium Embedded Framework (CEF)</a> Copyright &copy 2013 Marshall A. Greenblatt</center>" -nodeprecated -d %OUT_PATH% -link http://docs.oracle.com/javase/7/docs/api/ org.cef

:end
endlocal & set RC=%ERRORLEVEL%
goto omega

:returncode
exit /B %RC%

:omega
call :returncode %RC%
