// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef JCEF_NATIVE_FOCUS_HANDLER_H_
#define JCEF_NATIVE_FOCUS_HANDLER_H_
#pragma once

#include <jni.h>

#include "include/cef_focus_handler.h"

#include "jni_scoped_helpers.h"

// FocusHandler implementation.
class FocusHandler : public CefFocusHandler {
 public:
  FocusHandler(JNIEnv* env, jobject handler);

  // CefFocusHandler methods
  virtual void OnTakeFocus(CefRefPtr<CefBrowser> browser, bool next) override;
  virtual bool OnSetFocus(CefRefPtr<CefBrowser> browser,
                          FocusSource source) override;
  virtual void OnGotFocus(CefRefPtr<CefBrowser> browser) override;

 protected:
  ScopedJNIObjectGlobal handle_;

  // Include the default reference counting implementation.
  IMPLEMENT_REFCOUNTING(FocusHandler);
};

#endif  // JCEF_NATIVE_FOCUS_HANDLER_H_
