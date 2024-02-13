// Copyright (c) 2024 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef JCEF_NATIVE_DEVTOOLS_MESSAGE_OBSERVER_H_
#define JCEF_NATIVE_DEVTOOLS_MESSAGE_OBSERVER_H_
#pragma once

#include <jni.h>
#include "include/cef_devtools_message_observer.h"

#include "jni_scoped_helpers.h"

// DevToolsMessageObserver implementation.
class DevToolsMessageObserver : public CefDevToolsMessageObserver {
 public:
  DevToolsMessageObserver(JNIEnv* env, jobject observer);

  // CefDevToolsMessageObserver methods
  virtual void OnDevToolsMethodResult(CefRefPtr<CefBrowser> browser,
                                      int message_id,
                                      bool success,
                                      const void* result,
                                      size_t result_size) override;
  virtual void OnDevToolsEvent(CefRefPtr<CefBrowser> browser,
                               const CefString& method,
                               const void* params,
                               size_t params_size) override;

 protected:
  ScopedJNIObjectGlobal handle_;

  // Include the default reference counting implementation.
  IMPLEMENT_REFCOUNTING(DevToolsMessageObserver);
};

#endif  // JCEF_NATIVE_DEVTOOLS_MESSAGE_OBSERVER_H_
