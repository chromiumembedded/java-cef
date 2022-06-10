// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "critical_wait.h"

#include <sys/time.h>

// CriticalLock

CriticalLock::CriticalLock() {
  pthread_mutex_init(&lock_, nullptr);
}

CriticalLock::~CriticalLock() {
  pthread_mutex_destroy(&lock_);
}

void CriticalLock::Lock() {
  pthread_mutex_lock(&lock_);
}

void CriticalLock::Unlock() {
  pthread_mutex_unlock(&lock_);
}

// CriticalWait

CriticalWait::CriticalWait(CriticalLock* lock) : lock_(lock) {
  pthread_cond_init(&cond_, nullptr);
}

CriticalWait::~CriticalWait() {
  pthread_cond_destroy(&cond_);
}

void CriticalWait::Wait() {
  pthread_cond_wait(&cond_, &lock_->lock_);
}

bool CriticalWait::Wait(unsigned int maxWaitMs) {
  int sec = (int)(maxWaitMs / 1000);
  int nsec = (maxWaitMs - sec * 1000) * 1000000;  // convert to nsec
  struct timeval tv;
  struct timespec ts;

  gettimeofday(&tv, nullptr);
  ts.tv_sec = tv.tv_sec + sec;
  ts.tv_nsec = nsec;

  int res = pthread_cond_timedwait(&cond_, &lock_->lock_, &ts);
  return res == 0;
}

void CriticalWait::WakeUp() {
  pthread_cond_signal(&cond_);
}
