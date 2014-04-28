#!/bin/bash
# Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
# reserved. Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file.

if [ -z "$1" ]; then
  echo "ERROR: Please specify a target platform: linux32, linux64 or macosx64"
else
  ./make_jni_header.sh $1 org.cef.CefApp
  ./make_jni_header.sh $1 org.cef.browser.CefBrowser_N
  ./make_jni_header.sh $1 org.cef.callback.CefBeforeDownloadCallback_N
  ./make_jni_header.sh $1 org.cef.callback.CefContextMenuParams_N
  ./make_jni_header.sh $1 org.cef.callback.CefDownloadItem_N
  ./make_jni_header.sh $1 org.cef.callback.CefDownloadItemCallback_N
  ./make_jni_header.sh $1 org.cef.callback.CefDragData_N
  ./make_jni_header.sh $1 org.cef.callback.CefFileDialogCallback_N
  ./make_jni_header.sh $1 org.cef.callback.CefGeolocationCallback_N
  ./make_jni_header.sh $1 org.cef.callback.CefMenuModel_N
  ./make_jni_header.sh $1 org.cef.callback.CefQueryCallback_N
  ./make_jni_header.sh $1 org.cef.handler.CefClientHandler
  ./make_jni_header.sh $1 org.cef.handler.CefCommandLine_N
fi

