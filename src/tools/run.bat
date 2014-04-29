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

if "%3" == "" (
echo ERROR: Please specify a run type: detailed or simple
set ERRORLEVEL=1
goto end
)

set OUT_PATH=".\out\%1"
set LIB_PATH="%OUT_PATH%\%2"
set CLS_PATH=".\third_party\jogamp\jar\*;%OUT_PATH%"
set RUN_TYPE="%3"

:: Remove the first three params (%1, %2 and %3) and pass the rest to java.
shift
shift
shift

java -cp %CLS_PATH% -Djava.library.path=%LIB_PATH% tests.%RUN_TYPE%.MainFrame %*

:end
endlocal & set RC=%ERRORLEVEL%
goto omega

:returncode
exit /B %RC%

:omega
call :returncode %RC%
