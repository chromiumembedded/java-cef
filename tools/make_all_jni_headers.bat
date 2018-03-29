@echo off
:: Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
:: reserved. Use of this source code is governed by a BSD-style license
:: that can be found in the LICENSE file.

set RC=
setlocal

if "%1" == "" (
echo ERROR: Please specify a target platform: win32 or win64
set ERRORLEVEL=1
goto end
)

call make_jni_header.bat %1 org.cef.CefApp
call make_jni_header.bat %1 org.cef.browser.CefBrowser_N
call make_jni_header.bat %1 org.cef.browser.CefMessageRouter_N
call make_jni_header.bat %1 org.cef.browser.CefRequestContext_N
call make_jni_header.bat %1 org.cef.callback.CefAuthCallback_N
call make_jni_header.bat %1 org.cef.callback.CefBeforeDownloadCallback_N
call make_jni_header.bat %1 org.cef.callback.CefCommandLine_N
call make_jni_header.bat %1 org.cef.callback.CefCallback_N
call make_jni_header.bat %1 org.cef.callback.CefContextMenuParams_N
call make_jni_header.bat %1 org.cef.callback.CefDownloadItem_N
call make_jni_header.bat %1 org.cef.callback.CefDownloadItemCallback_N
call make_jni_header.bat %1 org.cef.callback.CefDragData_N
call make_jni_header.bat %1 org.cef.callback.CefFileDialogCallback_N
call make_jni_header.bat %1 org.cef.callback.CefJSDialogCallback_N
call make_jni_header.bat %1 org.cef.callback.CefMenuModel_N
call make_jni_header.bat %1 org.cef.callback.CefPrintDialogCallback_N
call make_jni_header.bat %1 org.cef.callback.CefPrintJobCallback_N
call make_jni_header.bat %1 org.cef.callback.CefQueryCallback_N
call make_jni_header.bat %1 org.cef.callback.CefRequestCallback_N
call make_jni_header.bat %1 org.cef.callback.CefSchemeRegistrar_N
call make_jni_header.bat %1 org.cef.handler.CefClientHandler
call make_jni_header.bat %1 org.cef.misc.CefPrintSettings_N
call make_jni_header.bat %1 org.cef.network.CefCookieManager_N
call make_jni_header.bat %1 org.cef.network.CefPostData_N
call make_jni_header.bat %1 org.cef.network.CefPostDataElement_N
call make_jni_header.bat %1 org.cef.network.CefRequest_N
call make_jni_header.bat %1 org.cef.network.CefResponse_N
call make_jni_header.bat %1 org.cef.network.CefURLRequest_N
call make_jni_header.bat %1 org.cef.network.CefWebPluginInfo_N
call make_jni_header.bat %1 org.cef.network.CefWebPluginManager_N

:end
endlocal & set RC=%ERRORLEVEL%
goto omega

:returncode
exit /B %RC%

:omega
call :returncode %RC%
