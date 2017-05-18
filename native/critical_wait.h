// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef JCEF_NATIVE_CRITICAL_WAIT_H_
#define JCEF_NATIVE_CRITICAL_WAIT_H_
#pragma once

#include "include/cef_task.h"

#if defined(OS_WIN)
#include <windows.h>
#define WAIT_MUTEX HANDLE
#define WAIT_COND HANDLE
#else
#include <pthread.h>
#define WAIT_MUTEX pthread_mutex_t
#define WAIT_COND pthread_cond_t
#endif

class CriticalLock {
 public:
  CriticalLock();
  ~CriticalLock();

  void Lock();
  void Unlock();

 private:
  friend class CriticalWait;
  WAIT_MUTEX lock_;
};

class CriticalWait {
 public:
  explicit CriticalWait(CriticalLock* lock);
  ~CriticalWait();

  void Wait();
  bool Wait(unsigned int maxWaitMs);
  void WakeUp();

  CriticalLock* lock() { return lock_; }

 private:
  WAIT_COND cond_;
  CriticalLock* lock_;
};

#endif  // JCEF_NATIVE_CRITICAL_WAIT_H_
