// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef JCEF_NATIVE_URL_REQUEST_CLIENT_H_
#define JCEF_NATIVE_URL_REQUEST_CLIENT_H_
#pragma once

#include <set>
#include <deque>
#include <jni.h>
#include "include/cef_urlrequest.h"

// CefURLRequestClient implementation.
class URLRequestClient : public CefURLRequestClient {
 private:
   URLRequestClient(JNIEnv* env,
                    jobject jURLRequestClient,
                    jobject jURLRequest);

 public:
  static CefRefPtr<URLRequestClient> Create(JNIEnv* env,
                                            jobject jRequestClient,
                                            jobject jURLRequest);
  virtual ~URLRequestClient();

  // CefURLRequestClient methods
  virtual void OnRequestComplete(CefRefPtr<CefURLRequest> request) OVERRIDE;

  virtual void OnUploadProgress(CefRefPtr<CefURLRequest> request,
                                int64 current,
                                int64 total) OVERRIDE;

  virtual void OnDownloadProgress(CefRefPtr<CefURLRequest> request,
                                  int64 current,
                                  int64 total) OVERRIDE;

  virtual void OnDownloadData(CefRefPtr<CefURLRequest> request,
                              const void* data,
                              size_t data_length) OVERRIDE;

  virtual bool GetAuthCredentials(bool isProxy,
                                  const CefString& host,
                                  int port,
                                  const CefString& realm,
                                  const CefString& scheme,
                                  CefRefPtr<CefAuthCallback> callback) OVERRIDE;

 protected:
  jobject jURLRequestClient_;
  jobject jURLRequest_;

  // Include the default reference counting implementation.
  IMPLEMENT_REFCOUNTING(URLRequestClient);
};

#endif  // JCEF_NATIVE_URL_REQUEST_CLIENT_H_
