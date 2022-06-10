// Copyright (c) 2019 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef JCEF_NATIVE_COOKIE_ACCESS_FILTER_H_
#define JCEF_NATIVE_COOKIE_ACCESS_FILTER_H_
#pragma once

#include <jni.h>
#include "include/cef_resource_request_handler.h"

#include "jni_scoped_helpers.h"

// CookieAccessFilter implementation.
class CookieAccessFilter : public CefCookieAccessFilter {
 public:
  CookieAccessFilter(JNIEnv* env, jobject handler);

  // CefCookieAccessFilter methods:
  bool CanSendCookie(CefRefPtr<CefBrowser> browser,
                     CefRefPtr<CefFrame> frame,
                     CefRefPtr<CefRequest> request,
                     const CefCookie& cookie) override;
  bool CanSaveCookie(CefRefPtr<CefBrowser> browser,
                     CefRefPtr<CefFrame> frame,
                     CefRefPtr<CefRequest> request,
                     CefRefPtr<CefResponse> response,
                     const CefCookie& cookie) override;

 protected:
  ScopedJNIObjectGlobal handle_;

  // Include the default reference counting implementation.
  IMPLEMENT_REFCOUNTING(CookieAccessFilter);
};

#endif  // JCEF_NATIVE_COOKIE_ACCESS_FILTER_H_
