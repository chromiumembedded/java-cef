#!/bin/bash
# Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
# reserved. Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file.

if [ -z "$1" ]; then
  echo "ERROR: Please specify a target platform: linux32, linux64 or macosx64"
else
  DIR="$( cd "$( dirname "$0" )" && cd .. && pwd )"
  DISTRIB_PATH="${DIR}/binary_distrib/$1"
  if [ ! -d "$DISTRIB_PATH" ]; then
    mkdir -p "$DISTRIB_PATH"
  fi

  # Create README.txt
  python "${DIR}"/tools/make_readme.py --output-dir "$DISTRIB_PATH/" --platform $1
fi
