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
    mkdir -p "$OUT_PATH"
  fi

  javac -Xdiags:verbose -cp $CLS_PATH -d $OUT_PATH java/tests/detailed/*.java java/tests/simple/*.java java/org/cef/*.java java/org/cef/browser/*.java java/org/cef/callback/*.java java/org/cef/handler/*.java java/org/cef/misc/*.java java/org/cef/network/*.java

  # Copy resource files.
  cp -f ./java/tests/detailed/handler/*.html $OUT_PATH/tests/detailed/handler
  cp -f ./java/tests/detailed/handler/*.png $OUT_PATH/tests/detailed/handler
fi

cd tools

