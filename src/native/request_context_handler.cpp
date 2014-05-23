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
