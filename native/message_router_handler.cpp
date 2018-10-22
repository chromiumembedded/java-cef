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

bool MessageRouterHandler::OnQuery(
    CefRefPtr<CefBrowser> browser,
    CefRefPtr<CefFrame> frame,
    int64 query_id,
    const CefString& request,
    bool persistent,
    CefRefPtr<CefMessageRouterBrowserSide::Callback> callback) {
  JNIEnv* env = GetJNIEnv();
  if (!env)
    return false;

  jobject query_callback =
      NewJNIObject(env, "org/cef/callback/CefQueryCallback_N");
  if (!query_callback)
    return false;
  SetCefForJNIObject(env, query_callback, callback.get(), "CefQueryCallback");

  jboolean jresult = JNI_FALSE;
  jstring jrequest = NewJNIString(env, request);
  jobject jframe = GetJNIFrame(env, frame);
  JNI_CALL_METHOD(env, jhandler_, "onQuery",
                  "(Lorg/cef/browser/CefBrowser;Lorg/cef/browser/"
                  "CefFrame;JLjava/lang/String;ZLorg/cef/"
                  "callback/CefQueryCallback;)Z",
                  Boolean, jresult, GetJNIBrowser(browser), jframe,
                  (jlong)query_id, jrequest, (jboolean)persistent,
                  query_callback);
  env->DeleteLocalRef(jrequest);
  bool result = (jresult != JNI_FALSE);
  if (!result) {
    // If the java method returns "false", the callback won't be used and
    // therefore the reference can be removed.
    SetCefForJNIObject<CefMessageRouterBrowserSide::Callback>(
        env, query_callback, NULL, "CefQueryCallback");
  }
  if (jframe)
    env->DeleteLocalRef(jframe);
  env->DeleteLocalRef(query_callback);
  return result;
}

void MessageRouterHandler::OnQueryCanceled(CefRefPtr<CefBrowser> browser,
                                           CefRefPtr<CefFrame> frame,
                                           int64 query_id) {
  JNIEnv* env = GetJNIEnv();
  if (!env)
    return;

  jobject jframe = GetJNIFrame(env, frame);
  JNI_CALL_VOID_METHOD(
      env, jhandler_, "onQueryCanceled",
      "(Lorg/cef/browser/CefBrowser;Lorg/cef/browser/CefFrame;J)V",
      GetJNIBrowser(browser), jframe, (jlong)query_id);
  if (jframe)
    env->DeleteLocalRef(jframe);
}
