// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "util.h"

#include <sstream>
#include <tchar.h>
#include <tlhelp32.h>
#include <windows.h>

#include "include/cef_path_util.h"

namespace util {

int GetPid() {
  return (int)GetCurrentProcessId();
}

int GetParentPid() {
  DWORD pid = GetCurrentProcessId();
  int ppid = 0;
  HANDLE hProcess;
  PROCESSENTRY32 pe32;

  hProcess = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );
  if (hProcess == INVALID_HANDLE_VALUE)
    return ppid;

  pe32.dwSize = sizeof(PROCESSENTRY32);
  if (!Process32First(hProcess, &pe32)) {
    CloseHandle(hProcess);
    return ppid;
  }

  do {
    if (pe32.th32ProcessID == pid) {
      ppid = (int)pe32.th32ParentProcessID;
      break;
    }
  } while(Process32Next(hProcess, &pe32));

  CloseHandle(hProcess);
  return ppid;
}

std::string GetTempFileName(const std::string& identifer, bool useParentId) {
  std::stringstream tmpName;
  CefString tmpPath;
  if (!CefGetPath(PK_DIR_TEMP, tmpPath)) {
    TCHAR lpPathBuffer[MAX_PATH];
    GetTempPath(MAX_PATH, lpPathBuffer);
    tmpPath.FromWString(lpPathBuffer);
  }
  tmpName << tmpPath.ToString().c_str() << "\\";
  tmpName << "jcef-p" << (useParentId ? util::GetParentPid() : util::GetPid());
  tmpName << (identifer.empty() ? "" : "_") << identifer.c_str() << ".tmp";
  return tmpName.str();
}

}  // namespace util
