#!/bin/bash
# Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
# reserved. Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file.

DIR="$( cd "$( dirname "$0" )" && cd .. && pwd )"
OUT_PATH="${DIR}/out/docs"

javadoc --ignore-source-errors -Xdoclint:none -windowtitle "CEF3 Java API Docs" -footer "<center><a href="https://bitbucket.org/chromiumembedded/java-cef" target="_top">Chromium Embedded Framework (CEF)</a> Copyright &copy 2013 Marshall A. Greenblatt</center>" -nodeprecated -d "$OUT_PATH" -sourcepath "${DIR}/java" -link http://docs.oracle.com/javase/7/docs/api/ -subpackages org.cef

