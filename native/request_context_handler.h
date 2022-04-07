// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef JCEF_NATIVE_STRING_VISITOR_H_
#define JCEF_NATIVE_STRING_VISITOR_H_
#pragma once

#include <jni.h>
#include "include/cef_request_context_handler.h"

#include "jni_scoped_helpers.h"

// RequestContextHandler implementation.
class RequestContextHandler : public CefRequestContextHandler {
 public:
  RequestContextHandler(JNIEnv* env, jobject jhandler);

  // RequestContextHandler methods
  CefRefPtr<CefResourceRequestHandler> GetResourceRequestHandler(
      CefRefPtr<CefBrowser> browser,
      CefRefPtr<CefFrame> frame,
      CefRefPtr<CefRequest> request,
      bool is_navigation,
      bool is_download,
      const CefString& request_initiator,
      bool& disable_default_handling) override;

 protected:
  ScopedJNIObjectGlobal handle_;

  // Include the default reference counting implementation.
  IMPLEMENT_REFCOUNTING(RequestContextHandler);
};

#endif  // JCEF_NATIVE_STRING_VISITOR_H_
