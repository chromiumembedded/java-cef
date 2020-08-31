#!/bin/bash
# Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
# reserved. Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file.

# Determine the absolute path to the current directory.
DIR="$( cd "$( dirname "$0" )" && pwd )"
BIN_DIR="${DIR}/bin"

# Compile the test program.
javac -cp "${BIN_DIR}:${BIN_DIR}/*" "${BIN_DIR}"/tests/detailed/*.java "${BIN_DIR}"/tests/detailed/dialog/*.java "${BIN_DIR}"/tests/detailed/handler/*.java "${BIN_DIR}"/tests/detailed/ui/*.java "${BIN_DIR}"/tests/simple/*.java

# Create the test JAR file.
jar -cf "${BIN_DIR}"/jcef-tests.jar -C "${BIN_DIR}" tests
