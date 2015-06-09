// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "CefQueryCallback_N.h"
#include "include/wrapper/cef_message_router.h"
#include "jni_util.h"

JNIEXPORT void JNICALL Java_org_cef_callback_CefQueryCallback_1N_N_1Success
  (JNIEnv *env, jobject obj, jstring response) {
  CefRefPtr<CefMessageRouterBrowserSide::Callback> callback =
      GetCefFromJNIObject<CefMessageRouterBrowserSide::Callback>(env, obj, "CefQueryCallback");
  if(!callback.get())
    return;

  callback->Success(GetJNIString(env, response));

  // Clear the reference added in ClientHandler::OnQuery.
  SetCefForJNIObject<CefMessageRouterBrowserSide::Callback>(env, obj, NULL, "CefQueryCallback");
}

JNIEXPORT void JNICALL Java_org_cef_callback_CefQueryCallback_1N_N_1Failure
  (JNIEnv *env, jobject obj, jint error_code, jstring error_message) {
  CefRefPtr<CefMessageRouterBrowserSide::Callback> callback =
      GetCefFromJNIObject<CefMessageRouterBrowserSide::Callback>(env, obj, "CefQueryCallback");
  if(!callback.get())
    return;

  callback->Failure(error_code, GetJNIString(env, error_message));

  // Clear the reference added in ClientHandler::OnQuery.
  SetCefForJNIObject<CefMessageRouterBrowserSide::Callback>(env, obj, NULL, "CefQueryCallback");
}
