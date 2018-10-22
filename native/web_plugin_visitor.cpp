// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "web_plugin_visitor.h"

#include "jni_util.h"
#include "util.h"

WebPluginInfoVisitor::WebPluginInfoVisitor(JNIEnv* env, jobject jvisitor) {
  jvisitor_ = env->NewGlobalRef(jvisitor);
}

WebPluginInfoVisitor::~WebPluginInfoVisitor() {
  JNIEnv* env = GetJNIEnv();
  env->DeleteGlobalRef(jvisitor_);
}

bool WebPluginInfoVisitor::Visit(CefRefPtr<CefWebPluginInfo> info,
                                 int count,
                                 int total) {
  JNIEnv* env = GetJNIEnv();
  if (!env)
    return false;

  jobject jinfo = NewJNIObject(env, "org/cef/network/CefWebPluginInfo_N");
  if (!jinfo)
    return false;
  SetCefForJNIObject(env, jinfo, info.get(), "CefWebPluginInfo");

  jboolean jresult = JNI_FALSE;
  JNI_CALL_METHOD(env, jvisitor_, "visit",
                  "(Lorg/cef/network/CefWebPluginInfo;II)Z", Boolean, jresult,
                  jinfo, (jint)count, (jint)total);

  SetCefForJNIObject<CefWebPluginInfo>(env, jinfo, NULL, "CefWebPluginInfo");
  env->DeleteLocalRef(jinfo);
  return jresult != JNI_FALSE;
}
