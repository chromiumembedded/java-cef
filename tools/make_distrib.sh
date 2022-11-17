#!/bin/bash
# Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
# reserved. Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file.

if [ -z "$1" ]; then
  echo "ERROR: Please specify a build target: linux32, linux64 or macosx64"
else
  DIR="$( cd "$( dirname "$0" )" && cd .. && pwd )"
  DISTRIB_PLATFORM="$1"
  DISTRIB_PATH="${DIR}/binary_distrib/$1"
  DISTRIB_BIN_PATH="$DISTRIB_PATH/bin"
  DISTRIB_DOCS_PATH="$DISTRIB_PATH/docs"
  OUT_PATH="${DIR}/out"
  OUT_DOCS_PATH="${DIR}/out/docs"
  SOURCE_PATH="${DIR}/java"
  JOGAMP_PATH="${DIR}/third_party/jogamp"
  TOOLS_DISTRIB_PATH="${DIR}/tools/distrib/$1"
  EXCLUDE_FILE="${DIR}/tools/distrib/EXCLUDE_FILES.txt"

  if [ ! -d "$DISTRIB_BIN_PATH" ]; then
    mkdir -p "$DISTRIB_BIN_PATH"
  fi

  # Create the JCEF documentation.
  "${DIR}"/tools/make_docs.sh

  # Copy documentation to the docs directory.
  cp -rf "$OUT_DOCS_PATH" "$DISTRIB_DOCS_PATH"

  # Create README.txt
  python "${DIR}"/tools/make_readme.py --output-dir "$DISTRIB_PATH/" --platform $DISTRIB_PLATFORM

  # Copy miscellaneous files to the root directory.
  cp -f "${DIR}"/LICENSE.txt "$DISTRIB_PATH"
  cp -f "$JOGAMP_PATH"/*.LICENSE.txt "$DISTRIB_PATH"
  rsync -a --exclude-from "$EXCLUDE_FILE" "$TOOLS_DISTRIB_PATH/" "$DISTRIB_PATH/"

  if [ $1 == "macosx64" ]; then
    OUT_BINARY_PATH="${DIR}/jcef_build/native/Release"
    if [ ! -d "$OUT_BINARY_PATH" ]; then
      echo "ERROR: Native Release build output path does not exist"
      exit 1
    fi

    # Copy test program source file to the tests directory.
    cp -rf "$SOURCE_PATH"/tests "$DISTRIB_PATH"

    # Everything else is contained in the app bundle.
    cp -rf "$OUT_BINARY_PATH"/jcef_app.app ""$DISTRIB_BIN_PATH
  else
    DISTRIB_LIB_PATH="$DISTRIB_PATH/bin/lib/$1"
    JOGAMP_JAR_PATH="$JOGAMP_PATH/jar"
    OUT_BINARY_PATH="$OUT_PATH/Release"

    # Alternately look in the CMake output path.
    if [ ! -d "$OUT_BINARY_PATH" ]; then
      OUT_BINARY_PATH="${DIR}/jcef_build/native/Release"
    fi
    if [ ! -d "$OUT_BINARY_PATH" ]; then
      echo "ERROR: Native Release build output path does not exist"
      exit 1
    fi
  
    # Create the JCEF JAR file.
    "${DIR}"/tools/make_jar.sh $1

    # Copy JAR files to the bin directory.
    cp -f "$JOGAMP_JAR_PATH"/gluegen-rt.jar "$DISTRIB_BIN_PATH"
    cp -f "$JOGAMP_JAR_PATH"/jogl-all.jar "$DISTRIB_BIN_PATH"

    if [ $1 == "linux32" ]; then
      JOGAMP_JAR_SUFFIX="i586"
    else
      JOGAMP_JAR_SUFFIX="amd64"
    fi
    cp -f "$JOGAMP_JAR_PATH"/gluegen-rt-natives-linux-$JOGAMP_JAR_SUFFIX.jar "$DISTRIB_BIN_PATH"
    cp -f "$JOGAMP_JAR_PATH"/jogl-all-natives-linux-$JOGAMP_JAR_SUFFIX.jar "$DISTRIB_BIN_PATH"
    cp -f "$OUT_PATH"/$1/jcef.jar "$DISTRIB_BIN_PATH"

    # Copy test program source and JAR file to the bin directory.
    cp -rf "$SOURCE_PATH"/tests "$DISTRIB_BIN_PATH"
    cp -f "$OUT_PATH"/$1/jcef-tests.jar "$DISTRIB_BIN_PATH"

    # Copy CEF Release files to the lib directory.
    if [ ! -d "$DISTRIB_LIB_PATH" ]; then
      mkdir -p "$DISTRIB_LIB_PATH"
    fi

    cp -f "$OUT_BINARY_PATH"/chrome-sandbox "$DISTRIB_LIB_PATH"
    cp -f "$OUT_BINARY_PATH"/libjcef.so "$DISTRIB_LIB_PATH"
    cp -f "$OUT_BINARY_PATH"/jcef_helper "$DISTRIB_LIB_PATH"
    cp -f "$OUT_BINARY_PATH"/icudtl.dat "$DISTRIB_LIB_PATH"
    cp -f "$OUT_BINARY_PATH"/libcef.so "$DISTRIB_LIB_PATH"
    cp -f "$OUT_BINARY_PATH"/libEGL.so "$DISTRIB_LIB_PATH"
    cp -f "$OUT_BINARY_PATH"/libGLESv2.so "$DISTRIB_LIB_PATH"
    cp -f "$OUT_BINARY_PATH"/snapshot_blob.bin "$DISTRIB_LIB_PATH"
    cp -f "$OUT_BINARY_PATH"/v8_context_snapshot.bin "$DISTRIB_LIB_PATH"
    cp -f "$OUT_BINARY_PATH"/*.pak "$DISTRIB_LIB_PATH"
    cp -rf "$OUT_BINARY_PATH"/locales/ "$DISTRIB_LIB_PATH"
    cp -f "$OUT_BINARY_PATH"/libvk_swiftshader.so "$DISTRIB_LIB_PATH"
    cp -f "$OUT_BINARY_PATH"/vk_swiftshader_icd.json "$DISTRIB_LIB_PATH"
    cp -f "$OUT_BINARY_PATH"/libvulkan.so.1 "$DISTRIB_LIB_PATH"
  fi
fi

