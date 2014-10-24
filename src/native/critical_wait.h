// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef JCEF_NATIVE_CRITICAL_WAIT_H_
#define JCEF_NATIVE_CRITICAL_WAIT_H_
#pragma once

#include "include/cef_runnable.h"
#include "include/cef_task.h"

#if defined(OS_WIN)
#define WAIT_COND HANDLE
#else
#include <pthread.h>
#define WAIT_COND pthread_cond_t
#endif

class CriticalWait {
 public:
  CriticalWait();
  virtual ~CriticalWait();

  void Wait();
  bool Wait(unsigned int maxWaitMs);
  void WakeUp();

 private:
  WAIT_COND cond_;

  IMPLEMENT_LOCKING(CriticalWait);
};

#endif  // JCEF_NATIVE_CRITICAL_WAIT_H_
