#!/bin/bash
# Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
# reserved. Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file.

# Necessary for jcef_helper to find libcef.so.
export LD_LIBRARY_PATH=./bin/lib/linux64

java -cp "./bin:./bin/*" -Djava.library.path=./bin/lib/linux64 MainFrame
