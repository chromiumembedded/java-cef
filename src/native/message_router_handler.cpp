// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "message_router_handler.h"

#include "client_handler.h"

#include "jni_util.h"
#include "util.h"

MessageRouterHandler::MessageRouterHandler(JNIEnv* env, jobject handler) {
  jhandler_ = env->NewGlobalRef(handler);
}

MessageRouterHandler::~MessageRouterHandler() {
  JNIEnv* env = GetJNIEnv();
  env->DeleteGlobalRef(jhandler_);
}

bool MessageRouterHandler::OnQuery(CefRefPtr<CefBrowser> browser,
                                   CefRefPtr<CefFrame> frame,
                                   int64 query_id,
                                   const CefString& request,
                                   bool persistent,
                                   CefRefPtr<CefMessageRouterBrowserSide::Callback> callback) {
  JNIEnv* env = GetJNIEnv();
  if (!env)
    return false;

  jobject query_callback = NewJNIObject(env, "org/cef/handler/CefQueryCallback_N");
  if (!query_callback)
    return false;
  SetCefForJNIObject(env, query_callback, callback.get(), "CefQueryCallback");

  jboolean result = JNI_FALSE;
  JNI_CALL_METHOD(env, jhandler_, 
                       "onQuery", 
                       "(Lorg/cef/browser/CefBrowser;JLjava/lang/String;ZLorg/cef/handler/CefQueryCallback;)Z",
                       Boolean,
                       result,
                       GetJNIBrowser(browser),
                       (jlong)query_id,
                       NewJNIString(env, request),
                       (jboolean)persistent,
                       query_callback);
  return (result != JNI_FALSE);
}

void MessageRouterHandler::OnQueryCanceled(CefRefPtr<CefBrowser> browser,
                                    CefRefPtr<CefFrame> frame,
                                    int64 query_id) {
  JNIEnv* env = GetJNIEnv();
  if (!env)
    return;

  JNI_CALL_VOID_METHOD(env, jhandler_, 
                       "onQueryCanceled", 
                       "(Lorg/cef/browser/CefBrowser;J)V",
                       GetJNIBrowser(browser),
                       (jlong)query_id);
}
