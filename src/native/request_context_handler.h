// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef CEF_TESTS_CEFCLIENT_STRING_VISITOR_H_
#define CEF_TESTS_CEFCLIENT_STRING_VISITOR_H_
#pragma once

#include <jni.h>
#include "include/cef_request_context.h"

// RequestContextHandler implementation.
class RequestContextHandler : public CefRequestContextHandler {
 public:
  RequestContextHandler(JNIEnv* env, jobject jhandler);
  virtual ~RequestContextHandler();

  // RequestContextHandler methods
  CefRefPtr<CefCookieManager> GetCookieManager() OVERRIDE;

 protected:
  jobject jhandler_;

  // Include the default reference counting implementation.
  IMPLEMENT_REFCOUNTING(RequestContextHandler);
};

#endif  // CEF_TESTS_CEFCLIENT_STRING_VISITOR_H_
