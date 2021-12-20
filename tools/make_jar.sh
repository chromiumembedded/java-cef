#!/bin/bash
# Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
# reserved. Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file.

if [ -z "$1" ]; then
  echo "ERROR: Please specify a build target: linux32 or linux64"
else
  DIR="$( cd "$( dirname "$0" )" && cd .. && pwd )"
  OUT_DIR="${DIR}/out/$1"
  jar -cmf "${OUT_DIR}"/manifest/MANIFEST.MF "${OUT_DIR}"/jcef.jar -C "${OUT_DIR}" org
  jar -cf "${OUT_DIR}"/jcef-tests.jar -C "${OUT_DIR}" tests
fi
