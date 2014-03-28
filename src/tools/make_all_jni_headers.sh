#!/bin/bash
# Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
# reserved. Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file.

if [ -z "$1" ]; then
  echo "ERROR: Please specify a target platform: linux32, linux64 or macosx64"
else
  ./make_jni_header.sh $1 CefBrowser_N
  ./make_jni_header.sh $1 CefClientHandler
  ./make_jni_header.sh $1 CefCommandLine_N
  ./make_jni_header.sh $1 CefApp
  ./make_jni_header.sh $1 CefQueryCallback_N
fi

