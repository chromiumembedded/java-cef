// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "CefAuthCallback_N.h"
#include "include/cef_request_handler.h"
#include "jni_util.h"

JNIEXPORT void JNICALL Java_org_cef_callback_CefAuthCallback_1N_N_1Continue
  (JNIEnv *env, jobject obj, jstring username, jstring password) {
  CefRefPtr<CefAuthCallback> callback =
      GetCefFromJNIObject<CefAuthCallback>(env, obj, "CefAuthCallback");
  if (!callback.get())
    return;
  callback->Continue(GetJNIString(env, username), GetJNIString(env, password));

  // Clear the reference added in RequestHandler::GetAuthCredentials
  SetCefForJNIObject<CefAuthCallback>(env, obj, NULL, "CefAuthCallback");
}

JNIEXPORT void JNICALL Java_org_cef_callback_CefAuthCallback_1N_N_1Cancel
  (JNIEnv *env, jobject obj) {
  CefRefPtr<CefAuthCallback> callback =
      GetCefFromJNIObject<CefAuthCallback>(env, obj, "CefAuthCallback");
  if (!callback.get())
    return;
  callback->Cancel();

  // Clear the reference added in RequestHandler::GetAuthCredentials
  SetCefForJNIObject<CefAuthCallback>(env, obj, NULL, "CefAuthCallback");
}
