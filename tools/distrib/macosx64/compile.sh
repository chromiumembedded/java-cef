#!/bin/bash
# Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
# reserved. Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file.

# Compile the test program.
javac -cp "./bin/jcef_app.app/Contents/Java/*":"." ./tests/detailed/*.java

# Create the test JAR file.
jar -cf tests/jcef-tests.jar tests/detailed/*.class tests/detailed/dialog/*.class tests/detailed/handler/*.class tests/detailed/ui/*.class

# Insert the test JAR file into the app bundle.
cp -f tests/jcef-tests.jar "./bin/jcef_app.app/Contents/Java/"
