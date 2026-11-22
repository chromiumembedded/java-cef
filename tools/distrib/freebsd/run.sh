#!/usr/local/bin/bash
# Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
# reserved. Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file.

# Determine the absolute path to the current directory.
DIR="$( cd "$( dirname "$0" )" && pwd )"
LIB_PATH="/usr/local/lib/jcef"

if [ -z "$1" ]; then
  EXAMPLE="detailed"
else
  EXAMPLE="$1"
fi

# Necessary for libjcef.so to find libjawt.so.
JAVA_PATH="/usr/local/openjdk25"
JAVA_LIB_PATH="/usr/local/openjdk25/lib"
if [ ! -d "$JAVA_LIB_PATH" ]; then
  echo "ERROR: Java lib path does not exist"
  exit 1
fi

# Without $LOCALBASE/lib JCEF won’t find the jogamp-jogl libraries
LIB_PATH="${LIB_PATH}:${JAVA_LIB_PATH}:/usr/local/lib"

# Necessary for jcef_helper to find libcef.so.
if [ -n "$LD_LIBRARY_PATH" ]; then
  LD_LIBRARY_PATH="$LIB_PATH:${LD_LIBRARY_PATH}"
else
  LD_LIBRARY_PATH="$LIB_PATH"
fi
export LD_LIBRARY_PATH

# Preload libcef.so to avoid crashes.
LD_PRELOAD=/usr/local/lib/jcef/libcef.so	\
/usr/local/openjdk25/bin/java --enable-native-access=ALL-UNNAMED	\
    --add-exports java.desktop/sun.awt=ALL-UNNAMED \
    --add-exports java.desktop/sun.java2d=ALL-UNNAMED \
    --add-exports java.base/java.lang=ALL-UNNAMED \
    -cp "/usr/local/share/jcef/jcef-tests.jar:/usr/local/share/java/classes/jcef.jar:/usr/local/share/java/classes/jogl2.jar:/usr/local/share/java/classes/gluegen2-rt.jar" \
    -Djava.library.path="$LIB_PATH" tests.${EXAMPLE}.MainFrame
