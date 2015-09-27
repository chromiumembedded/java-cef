#!/bin/bash
# Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
# reserved. Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file.

if [ -z "$1" ]; then
  echo "ERROR: Please specify a build target: linux32, linux64 or macosx64"
else
  cd ..

  export DISTRIB_PLATFORM="$1"
  export DISTRIB_PATH="./binary_distrib/$1"
  export DISTRIB_BIN_PATH="$DISTRIB_PATH/bin"
  export DISTRIB_DOCS_PATH="$DISTRIB_PATH/docs"
  export OUT_PATH="./out"
  export OUT_DOCS_PATH="./out/docs"
  export SOURCE_PATH="./java"
  export JOGAMP_PATH="./third_party/jogamp"
  export TOOLS_DISTRIB_PATH="./tools/distrib/$1"
  export EXCLUDE_FILE="./tools/distrib/EXCLUDE_FILES.txt"

  if [ ! -d "$DISTRIB_BIN_PATH" ]; then
    mkdir -p "$DISTRIB_BIN_PATH"
  fi

  # Create the JCEF documentation.
  cd tools
  ./make_docs.sh
  cd ..

  # Copy documentation to the docs directory.
  cp -rf $OUT_DOCS_PATH $DISTRIB_DOCS_PATH

  # Create README.txt
  python tools/make_readme.py --output-dir $DISTRIB_PATH/ --platform $DISTRIB_PLATFORM

  # Copy miscellaneous files to the root directory.
  cp -f ./LICENSE.txt $DISTRIB_PATH
  cp -f $JOGAMP_PATH/*.LICENSE.txt $DISTRIB_PATH
  rsync -a --exclude-from $EXCLUDE_FILE $TOOLS_DISTRIB_PATH/ $DISTRIB_PATH/

  if [ $1 == "macosx64" ]; then
    export OUT_BINARY_PATH="./jcef_build/native/Release"
    if [ ! -d "$OUT_BINARY_PATH" ]; then
      echo "ERROR: Native Release build output path does not exist"
      exit 1
    fi

    export DISTRIB_TESTS_PATH="$DISTRIB_PATH/tests"

    if [ ! -d "$DISTRIB_TESTS_PATH" ]; then
      mkdir -p "$DISTRIB_TESTS_PATH"
    fi

    # Copy test program source file to the tests directory.
    cp -rf $SOURCE_PATH/tests $DISTRIB_TESTS_PATH

    # Everything else is contained in the app bundle.
    cp -rf $OUT_BINARY_PATH/jcef_app.app $DISTRIB_BIN_PATH
  else
    export DISTRIB_LIB_PATH="$DISTRIB_PATH/bin/lib/$1"
    export JOGAMP_JAR_PATH="$JOGAMP_PATH/jar"
    export OUT_BINARY_PATH="$OUT_PATH/Release"

    # Alternately look in the CMake output path.
    if [ ! -d "$OUT_BINARY_PATH" ]; then
      export OUT_BINARY_PATH="./jcef_build/native/Release"
    fi
    if [ ! -d "$OUT_BINARY_PATH" ]; then
      echo "ERROR: Native Release build output path does not exist"
      exit 1
    fi
  
    # Create the JCEF JAR file.
    cd tools
    ./make_jar.sh $1
    cd ..

    # Copy JAR files to the bin directory.
    cp -f $JOGAMP_JAR_PATH/gluegen-rt.jar $DISTRIB_BIN_PATH
    cp -f $JOGAMP_JAR_PATH/jogl-all.jar $DISTRIB_BIN_PATH

    if [ $1 == "linux32" ]; then
      export JOGAMP_JAR_SUFFIX="i586"
    else
      export JOGAMP_JAR_SUFFIX="amd64"
    fi
    cp -f $JOGAMP_JAR_PATH/gluegen-rt-natives-linux-$JOGAMP_JAR_SUFFIX.jar $DISTRIB_BIN_PATH
    cp -f $JOGAMP_JAR_PATH/jogl-all-natives-linux-$JOGAMP_JAR_SUFFIX.jar $DISTRIB_BIN_PATH
    cp -f $OUT_PATH/$1/jcef.jar $DISTRIB_BIN_PATH

    # Copy test program source and JAR file to the bin directory.
    cp -rf $SOURCE_PATH/tests $DISTRIB_BIN_PATH
    cp -f $OUT_PATH/$1/jcef-tests.jar $DISTRIB_BIN_PATH

    # Copy CEF Release files to the lib directory.
    if [ ! -d "$DISTRIB_LIB_PATH" ]; then
      mkdir -p "$DISTRIB_LIB_PATH"
    fi

    cp -f $OUT_BINARY_PATH/chrome-sandbox $DISTRIB_LIB_PATH
    cp -f $OUT_BINARY_PATH/libjcef.so $DISTRIB_LIB_PATH
    cp -f $OUT_BINARY_PATH/jcef_helper $DISTRIB_LIB_PATH
    cp -f $OUT_BINARY_PATH/icudtl.dat $DISTRIB_LIB_PATH
    cp -f $OUT_BINARY_PATH/libcef.so $DISTRIB_LIB_PATH
    cp -f $OUT_BINARY_PATH/natives_blob.bin $DISTRIB_LIB_PATH
    cp -f $OUT_BINARY_PATH/snapshot_blob.bin $DISTRIB_LIB_PATH
    cp -f $OUT_BINARY_PATH/*.pak $DISTRIB_LIB_PATH
    cp -rf $OUT_BINARY_PATH/locales/ $DISTRIB_LIB_PATH
  fi

  cd tools
fi

