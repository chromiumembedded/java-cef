#!/bin/bash
# Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
# reserved. Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file.

if [ -z "$1" ]; then
  echo "ERROR: Please specify a target platform: linux32, linux64 or macosx64"
else
  DIR="$( cd "$( dirname "$0" )" && pwd )"
  "${DIR}"/make_jni_header.sh $1 org.cef.CefApp
  "${DIR}"/make_jni_header.sh $1 org.cef.browser.CefBrowser_N
  "${DIR}"/make_jni_header.sh $1 org.cef.browser.CefFrame_N
  "${DIR}"/make_jni_header.sh $1 org.cef.browser.CefMessageRouter_N
  "${DIR}"/make_jni_header.sh $1 org.cef.browser.CefRequestContext_N
  "${DIR}"/make_jni_header.sh $1 org.cef.callback.CefAuthCallback_N
  "${DIR}"/make_jni_header.sh $1 org.cef.callback.CefBeforeDownloadCallback_N
  "${DIR}"/make_jni_header.sh $1 org.cef.callback.CefCommandLine_N
  "${DIR}"/make_jni_header.sh $1 org.cef.callback.CefCallback_N
  "${DIR}"/make_jni_header.sh $1 org.cef.callback.CefContextMenuParams_N
  "${DIR}"/make_jni_header.sh $1 org.cef.callback.CefDownloadItem_N
  "${DIR}"/make_jni_header.sh $1 org.cef.callback.CefDownloadItemCallback_N
  "${DIR}"/make_jni_header.sh $1 org.cef.callback.CefDragData_N
  "${DIR}"/make_jni_header.sh $1 org.cef.callback.CefFileDialogCallback_N
  "${DIR}"/make_jni_header.sh $1 org.cef.callback.CefJSDialogCallback_N
  "${DIR}"/make_jni_header.sh $1 org.cef.callback.CefMenuModel_N
  "${DIR}"/make_jni_header.sh $1 org.cef.callback.CefPrintDialogCallback_N
  "${DIR}"/make_jni_header.sh $1 org.cef.callback.CefPrintJobCallback_N
  "${DIR}"/make_jni_header.sh $1 org.cef.callback.CefQueryCallback_N
  "${DIR}"/make_jni_header.sh $1 org.cef.callback.CefSchemeRegistrar_N
  "${DIR}"/make_jni_header.sh $1 org.cef.handler.CefClientHandler
  "${DIR}"/make_jni_header.sh $1 org.cef.misc.CefPrintSettings_N
  "${DIR}"/make_jni_header.sh $1 org.cef.network.CefCookieManager_N
  "${DIR}"/make_jni_header.sh $1 org.cef.network.CefPostData_N
  "${DIR}"/make_jni_header.sh $1 org.cef.network.CefPostDataElement_N
  "${DIR}"/make_jni_header.sh $1 org.cef.network.CefRequest_N
  "${DIR}"/make_jni_header.sh $1 org.cef.network.CefResponse_N
  "${DIR}"/make_jni_header.sh $1 org.cef.network.CefURLRequest_N
fi

