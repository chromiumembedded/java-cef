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
set LIB_PATH="%OUT_PATH%\%2"
set CLS_PATH=".\third_party\jogamp\jar\*;%OUT_PATH%"

:: Remove the first two params (%1 and %2) and pass the rest to java.
shift
shift

java -cp %CLS_PATH% -Djava.library.path=%LIB_PATH% tests.detailed.MainFrame %*

:end
endlocal & set RC=%ERRORLEVEL%
goto omega

:returncode
exit /B %RC%

:omega
call :returncode %RC%
