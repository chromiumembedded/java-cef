// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "web_plugin_visitor.h"

#include "jni_util.h"
#include "util.h"

WebPluginInfoVisitor::WebPluginInfoVisitor(JNIEnv* env, jobject jvisitor)
    : handle_(env, jvisitor) {}

bool WebPluginInfoVisitor::Visit(CefRefPtr<CefWebPluginInfo> info,
                                 int count,
                                 int total) {
  ScopedJNIEnv env;
  if (!env)
    return false;

  ScopedJNIObject<CefWebPluginInfo> jinfo(
      env, info, "org/cef/network/CefWebPluginInfo_N", "CefWebPluginInfo");
  jinfo.SetTemporary();

  jboolean jresult = JNI_FALSE;
  JNI_CALL_METHOD(env, handle_, "visit",
                  "(Lorg/cef/network/CefWebPluginInfo;II)Z", Boolean, jresult,
                  jinfo.get(), (jint)count, (jint)total);

  return jresult != JNI_FALSE;
}
