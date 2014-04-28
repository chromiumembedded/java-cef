#!/bin/bash
# Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
# reserved. Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file.

if [ -z "$1" ]; then
  echo "ERROR: Please specify a build target: linux32 or linux64"
else
  cd ../out/$1
  jar -cf jcef.jar org/cef/*.class org/cef/browser/*.class org/cef/callback/*.class org/cef/handler/*.class
  jar -cf jcef-tests.jar tests/*.class
  cd ../../tools
fi
