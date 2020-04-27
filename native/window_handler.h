// Copyright (c) 2015 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef JCEF_NATIVE_WINDOW_HANDLER_H_
#define JCEF_NATIVE_WINDOW_HANDLER_H_
#pragma once

#include <jni.h>

#include "include/cef_base.h"
#include "include/cef_browser.h"

#include "jni_scoped_helpers.h"

// WindowHandler implementation.
class WindowHandler : public CefBaseRefCounted {
 public:
  WindowHandler(JNIEnv* env, jobject handler);

  // WindowHandler methods
  bool GetRect(CefRefPtr<CefBrowser> browser, CefRect& rect);
  bool GetRect(jobject browser, CefRect& rect);
  void OnMouseEvent(CefRefPtr<CefBrowser> browser,
                    int mouseEvent,
                    int absX,
                    int absY,
                    int modifier,
                    int button);

 protected:
  ScopedJNIObjectGlobal handle_;

  // Include the default reference counting implementation.
  IMPLEMENT_REFCOUNTING(WindowHandler);
};

#endif  // JCEF_NATIVE_WINDOW_HANDLER_H_
