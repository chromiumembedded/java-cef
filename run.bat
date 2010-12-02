@echo off
setlocal

SET JDK_BIN="c:\jdk1.6\bin"
SET CLS_PATH="java\bin"

cd %CLS_PATH%
CALL %JDK_BIN%\java.exe -Djava.library.path=../lib/native/win32 org.limewire.cef.Main