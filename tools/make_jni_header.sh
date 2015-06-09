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
    export OUT_PATH="./native"
    export CLS_PATH="./third_party/jogamp/jar/*:./out/$1"
    export CLS_NAME="${2##*.}"

    javah -force -classpath $CLS_PATH -o $OUT_PATH/$CLS_NAME.h $2
  fi
fi

cd tools

