// Copyright (c) 2015 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef JCEF_NATIVE_TEMP_WINDOW_H_
#define JCEF_NATIVE_TEMP_WINDOW_H_
#pragma once

#if defined(OS_WIN)
#include "temp_window_win.h"
#elif defined(OS_LINUX)
#include "temp_window_x11.h"
#elif defined(OS_MACOSX)
#include "temp_window_mac.h"
#endif

#if defined(OS_WIN)
typedef TempWindowWin TempWindow;
#elif defined(OS_LINUX)
typedef TempWindowX11 TempWindow;
#elif defined(OS_MACOSX)
typedef TempWindowMac TempWindow;
#endif

#endif  // JCEF_NATIVE_TEMP_WINDOW_H_
