@echo off
set RC=
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

set OUT_PATH=".\out\%1"
set CLS_PATH=".\third_party\jogamp\jar\*;%OUT_PATH%"

java -cp %CLS_PATH% -Djava.library.path=%OUT_PATH%\%2 tests.MainFrame

:end
endlocal & set RC=%ERRORLEVEL%
goto omega

:returncode
exit /B %RC%

:omega
call :returncode %RC%
