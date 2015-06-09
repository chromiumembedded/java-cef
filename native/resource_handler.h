// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef JCEF_NATIVE_RESOURCE_HANDLER_H_
#define JCEF_NATIVE_RESOURCE_HANDLER_H_
#pragma once

#include <jni.h>
#include "include/cef_resource_handler.h"

// ResourceHandler implementation.
class ResourceHandler : public CefResourceHandler {
 public:
  ResourceHandler(JNIEnv* env, jobject handler);
  virtual ~ResourceHandler();

  // CefResourceHandler methods
  virtual bool ProcessRequest(CefRefPtr<CefRequest> request,
                              CefRefPtr<CefCallback> callback) OVERRIDE;
  virtual void GetResponseHeaders(CefRefPtr<CefResponse> response,
                                  int64& response_length,
                                  CefString& redirectUrl) OVERRIDE;
  virtual bool ReadResponse(void* data_out,
                            int bytes_to_read,
                            int& bytes_read,
                            CefRefPtr<CefCallback> callback) OVERRIDE;
  virtual bool CanGetCookie(const CefCookie& cookie) OVERRIDE;
  virtual bool CanSetCookie(const CefCookie& cookie) OVERRIDE;
  virtual void Cancel() OVERRIDE;

 protected:
  jobject jhandler_;

  // Include the default reference counting implementation.
  IMPLEMENT_REFCOUNTING(ResourceHandler);
};

#endif  // JCEF_NATIVE_RESOURCE_HANDLER_H_
