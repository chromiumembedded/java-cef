// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "CefWebPluginManager_N.h"

#include "web_plugin_visitor.h"
#include "web_plugin_unstable_callback.h"
#include "jni_util.h"

#include "include/cef_web_plugin.h"

JNIEXPORT void JNICALL Java_org_cef_network_CefWebPluginManager_1N_N_1VisitPlugins
  (JNIEnv *env, jobject, jobject jvisitor) {
  if (!jvisitor)
    return;
  CefRefPtr<CefWebPluginInfoVisitor> visitor = new WebPluginInfoVisitor(env, jvisitor);
  CefVisitWebPluginInfo(visitor);
}

JNIEXPORT void JNICALL Java_org_cef_network_CefWebPluginManager_1N_N_1RefreshPlugins
  (JNIEnv *, jobject) {
  CefRefreshWebPlugins();
}

JNIEXPORT void JNICALL Java_org_cef_network_CefWebPluginManager_1N_N_1UnregisterInternalPlugin
  (JNIEnv *env, jobject obj, jstring jpath) {
  if (!jpath)
    return;
  CefUnregisterInternalWebPlugin(GetJNIString(env, jpath));
}

JNIEXPORT void JNICALL Java_org_cef_network_CefWebPluginManager_1N_N_1RegisterPluginCrash
  (JNIEnv *env, jobject obj, jstring jpath) {
  if (!jpath)
    return;
  CefRegisterWebPluginCrash(GetJNIString(env, jpath));
}

JNIEXPORT void JNICALL Java_org_cef_network_CefWebPluginManager_1N_N_1IsWebPluginUnstable
  (JNIEnv *env, jobject obj, jstring jpath, jobject jcallback) {
  if (!jpath || !jcallback)
    return;
  CefRefPtr<CefWebPluginUnstableCallback> callback =
      new WebPluginUnstableCallback(env, jcallback);
  CefIsWebPluginUnstable(GetJNIString(env, jpath), callback);
}
