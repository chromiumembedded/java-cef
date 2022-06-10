// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef JCEF_NATIVE_COOKIE_VISITOR_H_
#define JCEF_NATIVE_COOKIE_VISITOR_H_
#pragma once

#include <jni.h>

#include "include/cef_cookie.h"

#include "jni_scoped_helpers.h"

// CookieVisitor implementation.
class CookieVisitor : public CefCookieVisitor {
 public:
  CookieVisitor(JNIEnv* env, jobject jvisitor);

  // CookieVisitor methods
  virtual bool Visit(const CefCookie& cookie,
                     int count,
                     int total,
                     bool& deleteCookie) override;

 protected:
  ScopedJNIObjectGlobal handle_;

  // Include the default reference counting implementation.
  IMPLEMENT_REFCOUNTING(CookieVisitor);
};

#endif  // JCEF_NATIVE_COOKIE_VISITOR_H_
