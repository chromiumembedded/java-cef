#!/bin/bash
# Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
# reserved. Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file.

cd ..

if [ -z "$1" ]; then
  echo "ERROR: Please specify a target platform: linux32 or linux64"
else
  if [ -z "$2" ]; then
    echo "ERROR: Please specify a class name"
  else
    export OUT_PATH="./native"
    export CLS_PATH="./third_party/jogamp/jar/*:./out/$1"

    javah -force -classpath $CLS_PATH -d $OUT_PATH org.cef.$2
    rm -f $OUT_PATH/$2.h
    mv $OUT_PATH/org_cef_$2.h $OUT_PATH/$2.h
  fi
fi

cd tools

