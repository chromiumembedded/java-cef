// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef CEF_TESTS_CEFCLIENT_FOCUS_HANDLER_H_
#define CEF_TESTS_CEFCLIENT_FOCUS_HANDLER_H_
#pragma once

#include <jni.h>
#include "include/cef_focus_handler.h"

// FocusHandler implementation.
class FocusHandler : public CefFocusHandler {
 public:
  FocusHandler(JNIEnv* env, jobject handler);
  virtual ~FocusHandler();

  // CefFocusHandler methods
  virtual void OnTakeFocus(CefRefPtr<CefBrowser> browser, bool next) OVERRIDE;
  virtual bool OnSetFocus(CefRefPtr<CefBrowser> browser, FocusSource source) OVERRIDE;
  virtual void OnGotFocus(CefRefPtr<CefBrowser> browser) OVERRIDE;

 protected:
  jobject jhandler_;

  // Include the default reference counting implementation.
  IMPLEMENT_REFCOUNTING(FocusHandler);
  // Include the default locking implementation.
  IMPLEMENT_LOCKING(FocusHandler);
};

#endif  // CEF_TESTS_CEFCLIENT_FOCUS_HANDLER_H_
