#!/bin/bash
# Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
# reserved. Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file.

cd ..

if [ -z "$1" ]; then
  echo "ERROR: Please specify a target platform: linux32 or linux64"
else
  if [ -z "$2" ]; then
    echo "ERROR: Please specify a build type: Debug or Release"
  else
    export OUT_PATH="./out"
    export LIB_PATH="$OUT_PATH/$2"
    export CLS_PATH="./third_party/jogamp/jar/*:$OUT_PATH/$1"

    # Necessary for jcef_helper to find libcef.so.
    export LD_LIBRARY_PATH=$LIB_PATH

    java -cp "$CLS_PATH" -Djava.library.path=$LIB_PATH MainFrame
  fi
fi

cd tools

