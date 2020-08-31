#!/bin/bash
# Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
# reserved. Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file.

# Determine the absolute path to the current directory.
DIR="$( cd "$( dirname "$0" )" && pwd )"
LIB_PATH="$DIR/bin/lib/linux32"

if [ -z "$1" ]; then
  EXAMPLE="detailed"
else
  EXAMPLE="$1"
fi

# Necessary for jcef_helper to find libcef.so.
if [ -n "$LD_LIBRARY_PATH" ]; then
  LD_LIBRARY_PATH="$LIB_PATH:${LD_LIBRARY_PATH}"
else
  LD_LIBRARY_PATH="$LIB_PATH"
fi
export LD_LIBRARY_PATH

# Preload libcef.so to avoid crashes.
LD_PRELOAD=libcef.so java -cp "${DIR}/bin:${DIR}/bin/*" -Djava.library.path="$LIB_PATH" tests.${EXAMPLE}.MainFrame "$@"
