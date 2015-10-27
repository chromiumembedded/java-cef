#!/bin/bash
# Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
# reserved. Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file.

cd ..

if [ -z "$1" ]; then
  echo "ERROR: Please specify a target platform: linux32, linux64 or macosx64"
else
  if [ -z "$2" ]; then
    echo "ERROR: Please specify a class name"
  else
    if [ $1 == "macosx64" ]; then
      export CLS_OUT_PATH="./jcef_build/native/Release"
      if [ ! -d "$CLS_OUT_PATH" ]; then
        export CLS_OUT_PATH="./jcef_build/native/Debug"
      fi
    else
      export CLS_OUT_PATH="./out/$1"
    fi

    export HEADER_PATH="./native"
    export CLS_PATH="./third_party/jogamp/jar/*:${CLS_OUT_PATH}"
    export CLS_NAME="${2##*.}"

    javah -force -classpath $CLS_PATH -o $HEADER_PATH/$CLS_NAME.h $2
  fi
fi

cd tools

