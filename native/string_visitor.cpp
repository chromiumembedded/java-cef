// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "string_visitor.h"

#include "jni_scoped_helpers.h"
#include "jni_util.h"
#include "util.h"

StringVisitor::StringVisitor(JNIEnv* env, jobject jvisitor)
    : handle_(env, jvisitor) {}

void StringVisitor::Visit(const CefString& string) {
  ScopedJNIEnv env;
  if (!env)
    return;

  ScopedJNIString jstring(env, string);
  JNI_CALL_VOID_METHOD(env, handle_, "visit", "(Ljava/lang/String;)V",
                       jstring.get());
}
