#!/bin/bash
# Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
# reserved. Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file.

cd ..

if [ -z "$1" ]; then
  echo "ERROR: Please specify a target platform: linux32 or linux64"
else
  export OUT_PATH="./out/$1"
  export CLS_PATH="./third_party/jogamp/jar/*:./java"

  if [ ! -d "$OUT_PATH" ]; then
    mkdir "$OUT_PATH"
  fi

  javac -cp $CLS_PATH -d $OUT_PATH java/tests/*.java java/org/cef/*.java
fi

cd tools

