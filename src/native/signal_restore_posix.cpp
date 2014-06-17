// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "signal_restore_posix.h"

#include <signal.h>
#include <string.h>

// arraysize borrowed from base/macros.h
template <typename T, size_t N>
char (&ArraySizeHelper(T (&array)[N]))[N];
#define arraysize(array) (sizeof(ArraySizeHelper(array)))

namespace {

const int signals_to_restore[] =
    {SIGHUP, SIGINT, SIGQUIT, SIGILL, SIGABRT, SIGFPE, SIGSEGV,
     SIGALRM, SIGTERM, SIGCHLD, SIGBUS, SIGTRAP, SIGPIPE};

struct sigaction signal_handlers[arraysize(signals_to_restore)];

}  // namespace

void BackupSignalHandlers() {
  struct sigaction sigact;
  for (unsigned i = 0; i < arraysize(signals_to_restore); ++i) {
    memset(&sigact, 0, sizeof(sigact));
    sigaction(signals_to_restore[i], NULL, &sigact);
    signal_handlers[i] = sigact;
  }
}

void RestoreSignalHandlers() {
  for (unsigned i = 0; i < arraysize(signals_to_restore); ++i) {
    sigaction(signals_to_restore[i], &signal_handlers[i], NULL);
  }
}
