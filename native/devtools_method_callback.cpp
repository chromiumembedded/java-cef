// Copyright (c) 2024 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "devtools_method_callback.h"

#include "jni_scoped_helpers.h"
#include "jni_util.h"
#include "util.h"

DevToolsMethodCallback::DevToolsMethodCallback(JNIEnv* env, jobject jcallback)
    : handle_(env, jcallback) {}

void DevToolsMethodCallback::onComplete(int generatedMessageId) {
  ScopedJNIEnv env;
  if (!env)
    return;

  JNI_CALL_VOID_METHOD(env, handle_, "onComplete", "(I)V",
                       (jint)generatedMessageId);
}
