// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "critical_wait.h"

// CriticalLock

CriticalLock::CriticalLock() {
  lock_ = CreateMutex(nullptr, FALSE, nullptr);
}

CriticalLock::~CriticalLock() {
  CloseHandle(lock_);
}

void CriticalLock::Lock() {
  WaitForSingleObject(lock_, INFINITE);
}

void CriticalLock::Unlock() {
  ReleaseMutex(lock_);
}

// CriticalWait

CriticalWait::CriticalWait(CriticalLock* lock) : lock_(lock) {
  cond_ = CreateEvent(nullptr, FALSE, FALSE, nullptr);
}

CriticalWait::~CriticalWait() {
  CloseHandle(cond_);
}

void CriticalWait::Wait() {
  lock_->Unlock();
  WaitForSingleObject(cond_, INFINITE);
  lock_->Lock();
}

bool CriticalWait::Wait(unsigned int maxWaitMs) {
  lock_->Unlock();
  DWORD result = WaitForSingleObject(cond_, (DWORD)maxWaitMs);
  lock_->Lock();
  return result != WAIT_FAILED;
}

void CriticalWait::WakeUp() {
  SetEvent(cond_);
}
