// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef JCEF_NATIVE_STRING_VISITOR_H_
#define JCEF_NATIVE_STRING_VISITOR_H_
#pragma once

#include <jni.h>
#include "include/cef_request_context_handler.h"

// RequestContextHandler implementation.
class RequestContextHandler : public CefRequestContextHandler {
 public:
  RequestContextHandler(JNIEnv* env, jobject jhandler);
  virtual ~RequestContextHandler();

  // RequestContextHandler methods
  CefRefPtr<CefCookieManager> GetCookieManager() OVERRIDE;

  bool OnBeforePluginLoad(const CefString& mime_type,
                          const CefString& plugin_url,
                          bool is_main_frame,
                          const CefString& top_origin_url,
                          CefRefPtr<CefWebPluginInfo> plugin_info,
                          PluginPolicy* plugin_policy) OVERRIDE;

 protected:
  jobject jhandler_;

  // Include the default reference counting implementation.
  IMPLEMENT_REFCOUNTING(RequestContextHandler);
};

#endif  // JCEF_NATIVE_STRING_VISITOR_H_
