// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "critical_wait.h"

CriticalWait::CriticalWait() {
  cond_ = CreateEvent(NULL, FALSE, FALSE, NULL);
}

CriticalWait::~CriticalWait() {
  CloseHandle(cond_);
}

void CriticalWait::Wait() {
  Unlock();
  WaitForSingleObject(cond_, INFINITE);
  Lock();
}

bool CriticalWait::Wait(unsigned int maxWaitMs) {
  Unlock();
  DWORD result = WaitForSingleObject(cond_, (DWORD)maxWaitMs);
  Lock();
  return result != WAIT_FAILED;
}

void CriticalWait::WakeUp() {
  SetEvent(cond_);
}
