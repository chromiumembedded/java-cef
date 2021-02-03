#!/bin/bash
# Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
# reserved. Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file.

if [ -z "$1" ]; then
  echo "ERROR: Please specify a binaries path"
else

mkdir $1/strip

cp $1/libcef.so $1/strip/libcef.so
cp $1/libEGL.so $1/strip/libEGL.so
cp $1/libGLESv2.so $1/strip/libGLESv2.so
cp $1/libjcef.so $1/strip/libjcef.so

strip -s $1/strip/libcef.so
strip -s $1/strip/libEGL.so
strip -s $1/strip/libGLESv2.so
strip -s $1/strip/libjcef.so
fi
