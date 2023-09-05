// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef JCEF_NATIVE_RESOURCE_HANDLER_H_
#define JCEF_NATIVE_RESOURCE_HANDLER_H_
#pragma once

#include <jni.h>

#include "include/cef_resource_handler.h"

#include "jni_scoped_helpers.h"

// ResourceHandler implementation.
class ResourceHandler : public CefResourceHandler {
 public:
  ResourceHandler(JNIEnv* env, jobject handler);

  // CefResourceHandler methods:
  bool ProcessRequest(CefRefPtr<CefRequest> request,
                      CefRefPtr<CefCallback> callback) override;
  void GetResponseHeaders(CefRefPtr<CefResponse> response,
                          int64_t& response_length,
                          CefString& redirectUrl) override;
  bool ReadResponse(void* data_out,
                    int bytes_to_read,
                    int& bytes_read,
                    CefRefPtr<CefCallback> callback) override;
  void Cancel() override;

 protected:
  ScopedJNIObjectGlobal handle_;

  // Include the default reference counting implementation.
  IMPLEMENT_REFCOUNTING(ResourceHandler);
};

#endif  // JCEF_NATIVE_RESOURCE_HANDLER_H_
