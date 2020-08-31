#!/bin/bash
# Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
# reserved. Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file.

if [ -z "$1" ]; then
  echo "ERROR: Please specify a target platform: linux32, linux64 or macosx64"
else
  if [ -z "$2" ]; then
    echo "ERROR: Please specify a class name"
  else
    DIR="$( cd "$( dirname "$0" )" && cd .. && pwd )"
    if [ $1 == "macosx64" ]; then
      CLS_OUT_PATH="${DIR}/jcef_build/native/Release"
      if [ ! -d "$CLS_OUT_PATH" ]; then
        CLS_OUT_PATH="${DIR}/jcef_build/native/Debug"
      fi
    else
      CLS_OUT_PATH="${DIR}/out/$1"
    fi

    HEADER_PATH="${DIR}/native"
    CLS_PATH="${DIR}/third_party/jogamp/jar/*:${CLS_OUT_PATH}"
    CLS_NAME="${2##*.}"

    javah -force -classpath "$CLS_PATH" -o "$HEADER_PATH/$CLS_NAME.h" $2
  fi
fi

