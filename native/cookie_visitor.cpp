// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "cookie_visitor.h"

#include "jni_scoped_helpers.h"
#include "jni_util.h"
#include "util.h"

CookieVisitor::CookieVisitor(JNIEnv* env, jobject jvisitor)
    : handle_(env, jvisitor) {}

bool CookieVisitor::Visit(const CefCookie& cookie,
                          int count,
                          int total,
                          bool& deleteCookie) {
  ScopedJNIEnv env;
  if (!env)
    return false;

  ScopedJNICookie jcookie(env, cookie);
  ScopedJNIBoolRef jdeleteCookie(env, deleteCookie);
  jboolean jresult = JNI_FALSE;

  JNI_CALL_METHOD(env, handle_, "visit",
                  "(Lorg/cef/network/CefCookie;IILorg/cef/misc/BoolRef;)Z",
                  Boolean, jresult, jcookie.get(), (jint)count, (jint)total,
                  jdeleteCookie.get());

  deleteCookie = jdeleteCookie;
  return (jresult != JNI_FALSE);
}
