// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "cookie_visitor.h"

#include "jni_util.h"
#include "util.h"

CookieVisitor::CookieVisitor(JNIEnv* env, jobject jvisitor) {
  jvisitor_ = env->NewGlobalRef(jvisitor);
}

CookieVisitor::~CookieVisitor() {
  JNIEnv* env = GetJNIEnv();
  env->DeleteGlobalRef(jvisitor_);
}

bool CookieVisitor::Visit(const CefCookie& cookie,
                          int count,
                          int total,
                          bool& deleteCookie) {
  JNIEnv* env = GetJNIEnv();
  if (!env)
    return false;

  jobject jdeleteCookie = NewJNIBoolRef(env, deleteCookie);
  jboolean jresult = JNI_FALSE;

  JNI_CALL_METHOD(env, jvisitor_,
                  "visit",
                  "(Lorg/cef/network/CefCookie;IILorg/cef/misc/BoolRef;)Z",
                  Boolean,
                  jresult,
                  NewJNICookie(env, cookie),
                  (jint)count,
                  (jint)total,
                  jdeleteCookie);

  deleteCookie = GetJNIBoolRef(env, jdeleteCookie);
  return (jresult != JNI_FALSE);
}
