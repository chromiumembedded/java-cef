// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "web_plugin_unstable_callback.h"

#include "jni_util.h"
#include "util.h"

WebPluginUnstableCallback::WebPluginUnstableCallback(JNIEnv* env,
                                                     jobject jcallback) {
  jcallback_ = env->NewGlobalRef(jcallback);
}

WebPluginUnstableCallback::~WebPluginUnstableCallback() {
  JNIEnv* env = GetJNIEnv();
  env->DeleteGlobalRef(jcallback_);
}

void WebPluginUnstableCallback::IsUnstable(const CefString& path,
                                           bool unstable) {
  JNIEnv* env = GetJNIEnv();
  if (!env)
    return;
  jstring jpath = NewJNIString(env, path);
  JNI_CALL_VOID_METHOD(env, jcallback_, "isUnstable", "(Ljava/lang/String;Z)V",
                       jpath, (unstable ? JNI_TRUE : JNI_FALSE));
  env->DeleteLocalRef(jpath);
}
