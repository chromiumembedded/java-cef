#!/bin/bash
# Copyright (c) 2019 The Chromium Embedded Framework Authors. All rights
# reserved. Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file.

cd ..

if [ -z "$1" ]; then
  echo "ERROR: Please specify a target platform: linux32 or linux64"
else
  if [ -z "$2" ]; then
    echo "ERROR: Please specify a build type: Debug or Release"
  else
    export OUT_PATH="./out/$1"

    export LIB_PATH=$(readlink -f "./jcef_build/native/$2")
    if [ ! -d "$LIB_PATH" ]; then
      echo "ERROR: Native build output path does not exist"
      exit 1
    fi

    export CLS_PATH="./third_party/jogamp/jar/*:$OUT_PATH"

    # Necessary for jcef_helper to find libcef.so.
    export LD_LIBRARY_PATH=$LIB_PATH

    # Remove the first two params ($1 and $2) and pass the rest to java.
    shift
    shift

    LD_PRELOAD=$LIB_PATH/libcef.so java -Djava.library.path=$LIB_PATH -jar ./third_party/junit/junit-platform-console-standalone-*.jar -cp $OUT_PATH --select-package tests.junittests "$@"
  fi
fi

cd tools
