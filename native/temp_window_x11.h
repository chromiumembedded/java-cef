// Copyright (c) 2015 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef JCEF_NATIVE_TEMP_WINDOW_X11_H_
#define JCEF_NATIVE_TEMP_WINDOW_X11_H_
#pragma once

#include "include/base/cef_scoped_ptr.h"
#include "include/cef_base.h"

// Represents a singleton hidden window that acts as a temporary parent for
// popup browsers.
class TempWindowX11 {
 public:
  // Returns the singleton window handle.
  static CefWindowHandle GetWindowHandle();

  static void* GetDisplay();

 private:
  // A single instance will be created/owned by Context.
  friend class Context;
  // Allow deletion via scoped_ptr only.
  friend struct base::DefaultDeleter<TempWindowX11>;

  TempWindowX11();
  ~TempWindowX11();

  CefWindowHandle xwindow_;

  DISALLOW_COPY_AND_ASSIGN(TempWindowX11);
};

#endif  // JCEF_NATIVE_TEMP_WINDOW_X11_H_
