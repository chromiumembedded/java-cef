#!/bin/bash
# Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
# reserved. Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file.

cd ../java

export OUT_PATH="../out/docs"

javadoc -windowtitle "CEF3 Java API Docs" -footer "<center><a href="https://bitbucket.org/chromiumembedded/java-cef" target="_top">Chromium Embedded Framework (CEF)</a> Copyright &copy 2013 Marshall A. Greenblatt</center>" -nodeprecated -d $OUT_PATH -link http://docs.oracle.com/javase/7/docs/api/ -subpackages org.cef

cd ../tools
