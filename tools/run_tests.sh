#!/bin/bash
# Copyright (c) 2019 The Chromium Embedded Framework Authors. All rights
# reserved. Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file.

if [ -z "$1" ]; then
  echo "ERROR: Please specify a target platform: linux32 or linux64"
else
  if [ -z "$2" ]; then
    echo "ERROR: Please specify a build type: Debug or Release"
  else
    DIR="$( cd "$( dirname "$0" )" && cd .. && pwd )"
    OUT_PATH="${DIR}/out/$1"

    LIB_PATH="${DIR}/jcef_build/native/$2"
    if [ ! -d "$LIB_PATH" ]; then
      echo "ERROR: Native build output path does not exist"
      exit 1
    fi

    CLS_PATH="${DIR}/third_party/jogamp/jar/*:$OUT_PATH"

    # Necessary for jcef_helper to find libcef.so.
    if [ -n "$LD_LIBRARY_PATH" ]; then
      LD_LIBRARY_PATH="$LIB_PATH:${LD_LIBRARY_PATH}"
    else
      LD_LIBRARY_PATH="$LIB_PATH"
    fi
    export LD_LIBRARY_PATH

    # Remove the first two params ($1 and $2) and pass the rest to java.
    shift
    shift

    LD_PRELOAD=libcef.so java -Djava.library.path="$LIB_PATH" -jar "${DIR}"/third_party/junit/junit-platform-console-standalone-*.jar -cp "$OUT_PATH" --select-package tests.junittests "$@"
  fi
fi

