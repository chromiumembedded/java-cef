#!/bin/bash
# Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
# reserved. Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file.

if [ -z "$1" ]; then
  echo "ERROR: Please specify a build target: Debug or Release"
else
  cd ..
  make -j8 jcef BUILDTYPE=$1
  cd tools
fi

