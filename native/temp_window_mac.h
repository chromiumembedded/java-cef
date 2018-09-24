// Copyright (c) 2015 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef JCEF_NATIVE_TEMP_WINDOW_MAC_H_
#define JCEF_NATIVE_TEMP_WINDOW_MAC_H_
#pragma once

#include "include/base/cef_scoped_ptr.h"
#include "include/cef_base.h"

#ifdef __OBJC__
@class NSWindow;
#else
class NSWindow;
#endif

// Represents a singleton hidden window that acts as a temporary parent for
// popup browsers.
class TempWindowMac {
 public:
  // Returns the singleton window handle.
  static CefWindowHandle GetWindowHandle();

  static NSWindow* GetWindow();

 private:
  // A single instance will be created/owned by Context.
  friend class Context;
  // Allow deletion via scoped_ptr only.
  friend struct base::DefaultDeleter<TempWindowMac>;

  TempWindowMac();
  ~TempWindowMac();

  NSWindow* window_;

  DISALLOW_COPY_AND_ASSIGN(TempWindowMac);
};

#endif  // JCEF_NATIVE_TEMP_WINDOW_MAC_H_
