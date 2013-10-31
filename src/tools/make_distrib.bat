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

set DISTRIB_PATH=".\binary_distrib\%1"
set DISTRIB_BIN_PATH="%DISTRIB_PATH%\bin"
set DISTRIB_DOCS_PATH="%DISTRIB_PATH%\docs"
set DISTRIB_LIB_PATH="%DISTRIB_PATH%\bin\lib\%1"
set OUT_PATH=".\out\%1"
set OUT_BINARY_PATH="%OUT_PATH%\Release"
set OUT_DOCS_PATH=".\out\docs"
set SOURCE_PATH=".\java"
set JOGAMP_PATH=".\third_party\jogamp"
set JOGAMP_JAR_PATH="%JOGAMP_PATH%\jar"
set TOOLS_DISTRIB_PATH=".\tools\distrib\%1"

:: Create the JCEF jar file.
cd tools
call make_jar.bat %1
cd ..

:: Create the JCEF documentation.
cd tools
call make_docs.bat
cd ..

:: Copy JAR files to the bin directory.
if not exist %DISTRIB_BIN_PATH% mkdir %DISTRIB_BIN_PATH%
xcopy /sfy %JOGAMP_JAR_PATH%\*.jar %DISTRIB_BIN_PATH%
copy %OUT_PATH%\jcef.jar %DISTRIB_BIN_PATH%

:: Copy test programs to the bin directory.
xcopy /sfy %SOURCE_PATH%\tests\*.java %DISTRIB_BIN_PATH%

:: Copy CEF Release files to the lib directory.
if not exist %DISTRIB_LIB_PATH% mkdir %DISTRIB_LIB_PATH%
copy %OUT_BINARY_PATH%\ffmpegsumo.dll %DISTRIB_LIB_PATH%
copy %OUT_BINARY_PATH%\icudt.dll %DISTRIB_LIB_PATH%
copy %OUT_BINARY_PATH%\jcef.dll %DISTRIB_LIB_PATH%
copy %OUT_BINARY_PATH%\jcef_helper.exe %DISTRIB_LIB_PATH%
copy %OUT_BINARY_PATH%\libcef.dll %DISTRIB_LIB_PATH%
xcopy /sfy %OUT_PATH%\Release\*.pak %DISTRIB_LIB_PATH%

:: Copy documentation to the docs directory.
xcopy /sfy %OUT_DOCS_PATH%\* %DISTRIB_DOCS_PATH%\

:: Copy miscellaneous files to the root directory.
copy .\LICENSE.txt %DISTRIB_PATH%
xcopy /sfy %JOGAMP_PATH%\*.LICENSE.txt %DISTRIB_PATH%
xcopy /sfy %TOOLS_DISTRIB_PATH%\* %DISTRIB_PATH%

:end
endlocal & set RC=%ERRORLEVEL%
goto omega

:returncode
exit /B %RC%

:omega
call :returncode %RC%
