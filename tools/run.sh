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
  elif [ -z "$3" ]; then
    echo "ERROR: Please specify a run type: detailed or simple"
  else
    export OUT_PATH="./out"

    export LIB_PATH=$(readlink -f "./jcef_build/native/$2")
    if [ ! -d "$LIB_PATH" ]; then
      echo "ERROR: Native build output path does not exist"
      exit 1
    fi

    export CLS_PATH="./third_party/jogamp/jar/*:$OUT_PATH/$1"
    export RUN_TYPE="$3"

    # Necessary for jcef_helper to find libcef.so.
    export LD_LIBRARY_PATH=$LIB_PATH

    # Remove the first three params ($1, $2 and $3) and pass the rest to java.
    shift
    shift
    shift

    LD_PRELOAD=$LIB_PATH/libcef.so java -cp "$CLS_PATH" -Djava.library.path=$LIB_PATH tests.$RUN_TYPE.MainFrame "$@"
  fi
fi

cd tools

