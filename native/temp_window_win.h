// Copyright (c) 2015 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef JCEF_NATIVE_TEMP_WINDOW_WIN_H_
#define JCEF_NATIVE_TEMP_WINDOW_WIN_H_
#pragma once

#include "include/cef_base.h"

// Represents a singleton hidden window that acts as a temporary parent for
// popup browsers.
class TempWindowWin {
 public:
  // Returns the singleton window handle.
  static CefWindowHandle GetWindowHandle();

 private:
  // A single instance will be created/owned by Context.
  friend class Context;
  // Allow deletion via std::unique_ptr only.
  friend std::default_delete<TempWindowWin>;

  TempWindowWin();
  ~TempWindowWin();

  CefWindowHandle hwnd_;

  DISALLOW_COPY_AND_ASSIGN(TempWindowWin);
};

#endif  // JCEF_NATIVE_TEMP_WINDOW_WIN_H_
