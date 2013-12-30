#!/bin/bash
# Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
# reserved. Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file.

if [ -z "$1" ]; then
  echo "ERROR: Please specify a build target: linux32, linux64 or macosx64"
else
  cd ..

  export DISTRIB_PATH="./binary_distrib/$1"
  export DISTRIB_BIN_PATH="$DISTRIB_PATH/bin"
  export DISTRIB_DOCS_PATH="$DISTRIB_PATH/docs"
  export DISTRIB_LIB_PATH="$DISTRIB_PATH/bin/lib/$1"
  export OUT_PATH="./out"
  export OUT_DOCS_PATH="./out/docs"
  export SOURCE_PATH="./java"
  export JOGAMP_PATH="./third_party/jogamp"
  export JOGAMP_JAR_PATH="$JOGAMP_PATH/jar"
  export TOOLS_DISTRIB_PATH="./tools/distrib/$1"

  if [ $1 == "macosx64" ]; then
    export OUT_BINARY_PATH="./xcodebuild/Release"
  else
    export OUT_BINARY_PATH="$OUT_PATH/Release"
  fi

  # Create the JCEF jar file.
  cd tools
  ./make_jar.sh $1
  cd ..

  # Create the JCEF documentation.
  cd tools
  ./make_docs.sh
  cd ..

  # Copy JAR files to the bin directory.
  if [ ! -d "$DISTRIB_BIN_PATH" ]; then
    mkdir -p "$DISTRIB_BIN_PATH"
  fi
  cp -f $JOGAMP_JAR_PATH/gluegen-rt.jar $DISTRIB_BIN_PATH
  cp -f $JOGAMP_JAR_PATH/jogl-all.jar $DISTRIB_BIN_PATH

  if [ $1 == "macosx64" ]; then
    cp -f $JOGAMP_JAR_PATH/gluegen-rt-natives-macosx-universal.jar $DISTRIB_BIN_PATH
    cp -f $JOGAMP_JAR_PATH/jogl-all-natives-macosx-universal.jar $DISTRIB_BIN_PATH
  else
    cp -f $JOGAMP_JAR_PATH/gluegen-rt-natives-linux-amd64.jar $DISTRIB_BIN_PATH
    cp -f $JOGAMP_JAR_PATH/jogl-all-natives-linux-amd64.jar $DISTRIB_BIN_PATH
  fi

  cp -f $OUT_PATH/$1/jcef.jar $DISTRIB_BIN_PATH

  # Copy test programs to the bin directory.
  cp -f $SOURCE_PATH/tests/*.java $DISTRIB_BIN_PATH

  # Copy CEF Release files to the lib directory.
  if [ ! -d "$DISTRIB_LIB_PATH" ]; then
    mkdir -p "$DISTRIB_LIB_PATH"
  fi

  if [ $1 == "macosx64" ]; then
    cp -rf $OUT_BINARY_PATH/jcef_app.app $DISTRIB_LIB_PATH
  else
    cp -f $OUT_BINARY_PATH/libffmpegsumo.so $DISTRIB_LIB_PATH
    cp -f $OUT_BINARY_PATH/libjcef.so $DISTRIB_LIB_PATH
    cp -f $OUT_BINARY_PATH/jcef_helper $DISTRIB_LIB_PATH
    cp -f $OUT_BINARY_PATH/libcef.so $DISTRIB_LIB_PATH
    cp -f $OUT_PATH/Release/*.pak $DISTRIB_LIB_PATH
    cp -rf $OUT_PATH/Release/locales/ $DISTRIB_LIB_PATH
  fi

  # Copy documentation to the docs directory.
  cp -rf $OUT_DOCS_PATH $DISTRIB_DOCS_PATH

  # Copy miscellaneous files to the root directory.
  cp -f ./LICENSE.txt $DISTRIB_PATH
  cp -f $JOGAMP_PATH/*.LICENSE.txt $DISTRIB_PATH
  cp -f $TOOLS_DISTRIB_PATH/* $DISTRIB_PATH

  cd tools
fi

