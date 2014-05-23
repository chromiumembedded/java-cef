// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "completion_handler.h"

#include "jni_util.h"
#include "util.h"

CompletionHandler::CompletionHandler(JNIEnv* env, jobject jhandler) {
  jhandler_ = env->NewGlobalRef(jhandler);
}

CompletionHandler::~CompletionHandler() {
  JNIEnv* env = GetJNIEnv();
  env->DeleteGlobalRef(jhandler_);
}

void CompletionHandler::OnComplete() {
  JNIEnv* env = GetJNIEnv();
  if (!env)
    return;
  JNI_CALL_VOID_METHOD(env, jhandler_, "onComplete", "()V");
}
