#!/bin/bash
# Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
# reserved. Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file.

javac -cp "./bin:./bin/*" ./bin/tests/detailed/*.java ./bin/tests/simple/*.java
