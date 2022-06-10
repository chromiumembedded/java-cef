// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "CefQueryCallback_N.h"
#include "include/wrapper/cef_message_router.h"
#include "jni_scoped_helpers.h"
#include "jni_util.h"

namespace {

using CefQueryCallback = CefMessageRouterBrowserSide::Callback;

CefRefPtr<CefQueryCallback> GetSelf(jlong self) {
  return reinterpret_cast<CefQueryCallback*>(self);
}

void ClearSelf(JNIEnv* env, jobject obj) {
  // Clear the reference added in ClientHandler::OnQuery.
  SetCefForJNIObject<CefQueryCallback>(env, obj, nullptr, "CefQueryCallback");
}

}  // namespace

JNIEXPORT void JNICALL
Java_org_cef_callback_CefQueryCallback_1N_N_1Success(JNIEnv* env,
                                                     jobject obj,
                                                     jlong self,
                                                     jstring response) {
  CefRefPtr<CefQueryCallback> callback = GetSelf(self);
  if (!callback)
    return;
  callback->Success(GetJNIString(env, response));
  ClearSelf(env, obj);
}

JNIEXPORT void JNICALL
Java_org_cef_callback_CefQueryCallback_1N_N_1Failure(JNIEnv* env,
                                                     jobject obj,
                                                     jlong self,
                                                     jint error_code,
                                                     jstring error_message) {
  CefRefPtr<CefQueryCallback> callback = GetSelf(self);
  if (!callback)
    return;
  callback->Failure(error_code, GetJNIString(env, error_message));
  ClearSelf(env, obj);
}
