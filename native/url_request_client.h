// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef JCEF_NATIVE_URL_REQUEST_CLIENT_H_
#define JCEF_NATIVE_URL_REQUEST_CLIENT_H_
#pragma once

#include <jni.h>
#include <deque>
#include <set>

#include "include/cef_urlrequest.h"

#include "jni_scoped_helpers.h"

// CefURLRequestClient implementation.
class URLRequestClient : public CefURLRequestClient {
 private:
  URLRequestClient(JNIEnv* env, jobject jURLRequestClient, jobject jURLRequest);

 public:
  static CefRefPtr<URLRequestClient> Create(JNIEnv* env,
                                            jobject jRequestClient,
                                            jobject jURLRequest);
  virtual ~URLRequestClient();

  // CefURLRequestClient methods
  virtual void OnRequestComplete(CefRefPtr<CefURLRequest> request) override;

  virtual void OnUploadProgress(CefRefPtr<CefURLRequest> request,
                                int64_t current,
                                int64_t total) override;

  virtual void OnDownloadProgress(CefRefPtr<CefURLRequest> request,
                                  int64_t current,
                                  int64_t total) override;

  virtual void OnDownloadData(CefRefPtr<CefURLRequest> request,
                              const void* data,
                              size_t data_length) override;

  virtual bool GetAuthCredentials(bool isProxy,
                                  const CefString& host,
                                  int port,
                                  const CefString& realm,
                                  const CefString& scheme,
                                  CefRefPtr<CefAuthCallback> callback) override;

 protected:
  ScopedJNIObjectGlobal client_handle_;
  ScopedJNIObjectGlobal request_handle_;

  // Include the default reference counting implementation.
  IMPLEMENT_REFCOUNTING(URLRequestClient);
};

#endif  // JCEF_NATIVE_URL_REQUEST_CLIENT_H_
