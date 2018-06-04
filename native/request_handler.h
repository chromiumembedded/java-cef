// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef JCEF_NATIVE_REQUEST_HANDLER_H_
#define JCEF_NATIVE_REQUEST_HANDLER_H_
#pragma once

#include <jni.h>
#include "include/cef_request_handler.h"

// RequestHandler implementation.
class RequestHandler : public CefRequestHandler {
 public:
  RequestHandler(JNIEnv* env, jobject handler);
  virtual ~RequestHandler();

  // CefRequestHandler methods
  virtual bool OnBeforeBrowse(CefRefPtr<CefBrowser> browser,
                              CefRefPtr<CefFrame> frame,
                              CefRefPtr<CefRequest> request,
                              bool user_gesture,
                              bool is_redirect) OVERRIDE;

  virtual ReturnValue OnBeforeResourceLoad(
      CefRefPtr<CefBrowser> browser,
      CefRefPtr<CefFrame> frame,
      CefRefPtr<CefRequest> request,
      CefRefPtr<CefRequestCallback> callback) OVERRIDE;

  virtual CefRefPtr<CefResourceHandler> GetResourceHandler(
      CefRefPtr<CefBrowser> browser,
      CefRefPtr<CefFrame> frame,
      CefRefPtr<CefRequest> request) OVERRIDE;

  virtual void OnResourceRedirect(CefRefPtr<CefBrowser> browser,
                                  CefRefPtr<CefFrame> frame,
                                  CefRefPtr<CefRequest> request,
                                  CefRefPtr<CefResponse> response,
                                  CefString& new_url) OVERRIDE;

  virtual bool OnResourceResponse(CefRefPtr<CefBrowser> browser,
                                  CefRefPtr<CefFrame> frame,
                                  CefRefPtr<CefRequest> request,
                                  CefRefPtr<CefResponse> response) OVERRIDE;

  virtual void OnResourceLoadComplete(CefRefPtr<CefBrowser> browser,
                                      CefRefPtr<CefFrame> frame,
                                      CefRefPtr<CefRequest> request,
                                      CefRefPtr<CefResponse> response,
                                      URLRequestStatus status,
                                      int64 received_content_length) OVERRIDE;

  virtual bool GetAuthCredentials(CefRefPtr<CefBrowser> browser,
                                  CefRefPtr<CefFrame> frame,
                                  bool isProxy,
                                  const CefString& host,
                                  int port,
                                  const CefString& realm,
                                  const CefString& scheme,
                                  CefRefPtr<CefAuthCallback> callback) OVERRIDE;

  virtual bool OnQuotaRequest(CefRefPtr<CefBrowser> browser,
                              const CefString& origin_url,
                              int64 new_size,
                              CefRefPtr<CefRequestCallback> callback) OVERRIDE;

  virtual void OnProtocolExecution(CefRefPtr<CefBrowser> browser,
                                   const CefString& url,
                                   bool& allow_os_execution) OVERRIDE;

  virtual bool OnCertificateError(
      CefRefPtr<CefBrowser> browser,
      cef_errorcode_t cert_error,
      const CefString& request_url,
      CefRefPtr<CefSSLInfo> ssl_info,
      CefRefPtr<CefRequestCallback> callback) OVERRIDE;

  virtual void OnPluginCrashed(CefRefPtr<CefBrowser> browser,
                               const CefString& plugin_path) OVERRIDE;

  virtual void OnRenderProcessTerminated(CefRefPtr<CefBrowser> browser,
                                         TerminationStatus status) OVERRIDE;

 protected:
  jobject jhandler_;

  // Include the default reference counting implementation.
  IMPLEMENT_REFCOUNTING(RequestHandler);
};

#endif  // JCEF_NATIVE_REQUEST_HANDLER_H_
