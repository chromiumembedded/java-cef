// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef JCEF_NATIVE_DISPLAY_HANDLER_H_
#define JCEF_NATIVE_DISPLAY_HANDLER_H_
#pragma once

#include <jni.h>

#include "include/cef_display_handler.h"

#include "jni_scoped_helpers.h"

// DisplayHandler implementation.
class DisplayHandler : public CefDisplayHandler {
 public:
  DisplayHandler(JNIEnv* env, jobject handler);

  // CefDisplayHandler methods:
  void OnAddressChange(CefRefPtr<CefBrowser> browser,
                       CefRefPtr<CefFrame> frame,
                       const CefString& url) OVERRIDE;
  void OnTitleChange(CefRefPtr<CefBrowser> browser,
                     const CefString& title) OVERRIDE;
  bool OnTooltip(CefRefPtr<CefBrowser> browser, CefString& text) OVERRIDE;
  void OnStatusMessage(CefRefPtr<CefBrowser> browser,
                       const CefString& value) OVERRIDE;
  bool OnConsoleMessage(CefRefPtr<CefBrowser> browser,
                        cef_log_severity_t level,
                        const CefString& message,
                        const CefString& source,
                        int line) OVERRIDE;

 protected:
  ScopedJNIObjectGlobal handle_;

  // Include the default reference counting implementation.
  IMPLEMENT_REFCOUNTING(DisplayHandler);
};

#endif  // JCEF_NATIVE_DISPLAY_HANDLER_H_
