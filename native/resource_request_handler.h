// Copyright (c) 2019 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef JCEF_NATIVE_RESOURCE_REQUEST_HANDLER_H_
#define JCEF_NATIVE_RESOURCE_REQUEST_HANDLER_H_
#pragma once

#include <jni.h>
#include "include/cef_resource_request_handler.h"

#include "jni_scoped_helpers.h"

// ResourceRequestHandler implementation.
class ResourceRequestHandler : public CefResourceRequestHandler {
 public:
  ResourceRequestHandler(JNIEnv* env, jobject handler);

  // CefResourceRequestHandler methods:
  CefRefPtr<CefCookieAccessFilter> GetCookieAccessFilter(
      CefRefPtr<CefBrowser> browser,
      CefRefPtr<CefFrame> frame,
      CefRefPtr<CefRequest> request) override;
  ReturnValue OnBeforeResourceLoad(CefRefPtr<CefBrowser> browser,
                                   CefRefPtr<CefFrame> frame,
                                   CefRefPtr<CefRequest> request,
                                   CefRefPtr<CefCallback> callback) override;
  CefRefPtr<CefResourceHandler> GetResourceHandler(
      CefRefPtr<CefBrowser> browser,
      CefRefPtr<CefFrame> frame,
      CefRefPtr<CefRequest> request) override;
  void OnResourceRedirect(CefRefPtr<CefBrowser> browser,
                          CefRefPtr<CefFrame> frame,
                          CefRefPtr<CefRequest> request,
                          CefRefPtr<CefResponse> response,
                          CefString& new_url) override;
  bool OnResourceResponse(CefRefPtr<CefBrowser> browser,
                          CefRefPtr<CefFrame> frame,
                          CefRefPtr<CefRequest> request,
                          CefRefPtr<CefResponse> response) override;
  void OnResourceLoadComplete(CefRefPtr<CefBrowser> browser,
                              CefRefPtr<CefFrame> frame,
                              CefRefPtr<CefRequest> request,
                              CefRefPtr<CefResponse> response,
                              URLRequestStatus status,
                              int64_t received_content_length) override;
  void OnProtocolExecution(CefRefPtr<CefBrowser> browser,
                           CefRefPtr<CefFrame> frame,
                           CefRefPtr<CefRequest> request,
                           bool& allow_os_execution) override;

 protected:
  ScopedJNIObjectGlobal handle_;

  // Include the default reference counting implementation.
  IMPLEMENT_REFCOUNTING(ResourceRequestHandler);
};

#endif  // JCEF_NATIVE_RESOURCE_REQUEST_HANDLER_H_
