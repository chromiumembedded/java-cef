// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "request_context_handler.h"

#include "jni_util.h"
#include "util.h"

RequestContextHandler::RequestContextHandler(JNIEnv* env, jobject jhandler) {
  jhandler_ = env->NewGlobalRef(jhandler);
}

RequestContextHandler::~RequestContextHandler() {
  JNIEnv* env = GetJNIEnv();
  env->DeleteGlobalRef(jhandler_);
}

CefRefPtr<CefCookieManager> RequestContextHandler::GetCookieManager() {
  JNIEnv* env = GetJNIEnv();
  if (!env)
    return NULL;
  jobject jresult = NULL;
  JNI_CALL_METHOD(env, jhandler_,
                       "getCookieManager",
                       "()Lorg/cef/network/CefCookieManager;",
                       Object,
                       jresult);
  if (!jresult)
    return NULL;
  CefRefPtr<CefCookieManager> result =
      GetCefFromJNIObject<CefCookieManager>(env, jresult, "CefCookieManager");
  return result;
}

bool RequestContextHandler::OnBeforePluginLoad(
    const CefString& mime_type,
    const CefString& plugin_url,
    bool is_main_frame,
    const CefString& top_origin_url,
    CefRefPtr<CefWebPluginInfo> plugin_info,
    PluginPolicy* plugin_policy) {
  JNIEnv* env = GetJNIEnv();
  if (!env)
    return false;

  jobject jinfo = NewJNIObject(env, "org/cef/network/CefWebPluginInfo_N");
  if (!jinfo)
    return false;
  SetCefForJNIObject(env, jinfo, plugin_info.get(), "CefWebPluginInfo");

  jboolean jresult = JNI_FALSE;
  JNI_CALL_METHOD(env, jhandler_,
                       "onBeforePluginLoad",
                       "Ljava/lang/String;Ljava/lang/String;ZLjava/lang/String;Lorg/cef/network/CefWebPluginInfo;)Z",
                       Boolean,
                       jresult,
                       NewJNIString(env, mime_type),
                       NewJNIString(env, plugin_url),
                       is_main_frame ? JNI_TRUE : JNI_FALSE,
                       NewJNIString(env, top_origin_url),
                       jinfo);

  SetCefForJNIObject<CefWebPluginInfo>(env, jinfo, NULL, "CefWebPluginInfo");

  if (jresult == JNI_FALSE) {
    // Allow the plugin load.
    if (*plugin_policy != PLUGIN_POLICY_ALLOW &&
        *plugin_policy != PLUGIN_POLICY_DETECT_IMPORTANT) {
      *plugin_policy = PLUGIN_POLICY_ALLOW;
      return true;
    }
  } else {
    // Block the plugin load.
    if (*plugin_policy != PLUGIN_POLICY_BLOCK &&
        *plugin_policy != PLUGIN_POLICY_DISABLE) {
      *plugin_policy = PLUGIN_POLICY_BLOCK;
      return true;
    }
  }

  return false;
}
