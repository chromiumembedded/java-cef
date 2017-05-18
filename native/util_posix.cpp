// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "util.h"

#include <unistd.h>
#include <sstream>

#include "include/cef_path_util.h"

namespace util {

int GetPid() {
  return getpid();
}

int GetParentPid() {
  return getppid();
}

std::string GetTempFileName(const std::string& identifer, bool useParentId) {
  std::stringstream tmpName;
  CefString tmpPath;
  if (!CefGetPath(PK_DIR_TEMP, tmpPath))
    tmpPath = "/tmp/";
  tmpName << tmpPath.ToString().c_str();
  tmpName << "jcef-p" << (useParentId ? util::GetParentPid() : util::GetPid());
  tmpName << (identifer.empty() ? "" : "_") << identifer.c_str() << ".tmp";
  return tmpName.str();
}

}  // namespace util
