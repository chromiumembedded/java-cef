// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "string_visitor.h"

#include "jni_util.h"
#include "util.h"

StringVisitor::StringVisitor(JNIEnv* env, jobject jvisitor) {
  jvisitor_ = env->NewGlobalRef(jvisitor);
}

StringVisitor::~StringVisitor() {
  JNIEnv* env = GetJNIEnv();
  env->DeleteGlobalRef(jvisitor_);
}

void StringVisitor::Visit(const CefString& string) {
  JNIEnv* env = GetJNIEnv();
  if (!env)
    return;
  jstring j_string = NewJNIString(env, string);
  JNI_CALL_VOID_METHOD(env, jvisitor_, "visit", "(Ljava/lang/String;)V",
                       j_string);
  env->DeleteLocalRef(j_string);
}
