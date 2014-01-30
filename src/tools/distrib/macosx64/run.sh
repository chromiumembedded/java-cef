#!/bin/bash
# Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
# reserved. Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file.

java -cp "./bin:./bin/*" -Djava.library.path=./bin/lib/macosx64/jcef_app.app/Contents/MacOS tests.MainFrame
