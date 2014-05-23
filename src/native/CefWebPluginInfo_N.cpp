// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "CefWebPluginInfo_N.h"
#include "jni_util.h"

#include "include/cef_web_plugin.h"

JNIEXPORT jstring JNICALL Java_org_cef_network_CefWebPluginInfo_1N_N_1GetName
  (JNIEnv *env, jobject obj) {
  CefRefPtr<CefWebPluginInfo> info =
      GetCefFromJNIObject<CefWebPluginInfo>(env, obj, "CefWebPluginInfo");
  if (!info.get())
    return NULL;
  return NewJNIString(env, info->GetName());
}

JNIEXPORT jstring JNICALL Java_org_cef_network_CefWebPluginInfo_1N_N_1GetPath
  (JNIEnv *env, jobject obj) {
  CefRefPtr<CefWebPluginInfo> info =
      GetCefFromJNIObject<CefWebPluginInfo>(env, obj, "CefWebPluginInfo");
  if (!info.get())
    return NULL;
  return NewJNIString(env, info->GetPath());
}

JNIEXPORT jstring JNICALL Java_org_cef_network_CefWebPluginInfo_1N_N_1GetVersion
  (JNIEnv *env, jobject obj) {
  CefRefPtr<CefWebPluginInfo> info =
      GetCefFromJNIObject<CefWebPluginInfo>(env, obj, "CefWebPluginInfo");
  if (!info.get())
    return NULL;
  return NewJNIString(env, info->GetVersion());
}

JNIEXPORT jstring JNICALL Java_org_cef_network_CefWebPluginInfo_1N_N_1GetDescription
  (JNIEnv *env, jobject obj) {
  CefRefPtr<CefWebPluginInfo> info =
      GetCefFromJNIObject<CefWebPluginInfo>(env, obj, "CefWebPluginInfo");
  if (!info.get())
    return NULL;
  return NewJNIString(env, info->GetDescription());
}
