@echo off
setlocal

SET JDK_BIN="c:\jdk1.6\bin"
SET CLS_PATH="java\bin"
SET OUT_PATH="native\src"

CALL %JDK_BIN%\javah.exe -force -classpath %CLS_PATH% -d %OUT_PATH% org.limewire.cef.%1
del /F %OUT_PATH%\%1.h
rename %OUT_PATH%\org_limewire_cef_%1.h %1.h
