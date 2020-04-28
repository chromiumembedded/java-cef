// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "web_plugin_unstable_callback.h"

#include "jni_util.h"
#include "util.h"

WebPluginUnstableCallback::WebPluginUnstableCallback(JNIEnv* env,
                                                     jobject jcallback)
    : handle_(env, jcallback) {}

void WebPluginUnstableCallback::IsUnstable(const CefString& path,
                                           bool unstable) {
  ScopedJNIEnv env;
  if (!env)
    return;

  ScopedJNIString jpath(env, path);

  JNI_CALL_VOID_METHOD(env, handle_, "isUnstable", "(Ljava/lang/String;Z)V",
                       jpath.get(), (unstable ? JNI_TRUE : JNI_FALSE));
}
