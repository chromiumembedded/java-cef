// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "completion_callback.h"

#include "jni_scoped_helpers.h"
#include "jni_util.h"
#include "util.h"

CompletionCallback::CompletionCallback(JNIEnv* env, jobject jhandler)
    : handle_(env, jhandler) {}

void CompletionCallback::OnComplete() {
  ScopedJNIEnv env;
  if (!env)
    return;
  JNI_CALL_VOID_METHOD(env, handle_, "onComplete", "()V");
}
