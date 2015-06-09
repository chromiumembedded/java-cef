// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef SIGNAL_RESTORE_POSIX_H
#define SIGNAL_RESTORE_POSIX_H

void BackupSignalHandlers();
void RestoreSignalHandlers();

#endif // SIGNAL_RESTORE_POSIX_H
