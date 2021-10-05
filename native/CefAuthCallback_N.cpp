// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "CefAuthCallback_N.h"
#include "include/cef_request_handler.h"
#include "jni_scoped_helpers.h"
#include "jni_util.h"

namespace {

CefRefPtr<CefAuthCallback> GetSelf(jlong self) {
  return reinterpret_cast<CefAuthCallback*>(self);
}

void ClearSelf(JNIEnv* env, jobject obj) {
  // Clear the reference added in RequestHandler::GetAuthCredentials.
  SetCefForJNIObject<CefAuthCallback>(env, obj, nullptr, "CefAuthCallback");
}

}  // namespace

JNIEXPORT void JNICALL
Java_org_cef_callback_CefAuthCallback_1N_N_1Continue(JNIEnv* env,
                                                     jobject obj,
                                                     jlong self,
                                                     jstring username,
                                                     jstring password) {
  CefRefPtr<CefAuthCallback> callback = GetSelf(self);
  if (!callback)
    return;
  callback->Continue(GetJNIString(env, username), GetJNIString(env, password));
  ClearSelf(env, obj);
}

JNIEXPORT void JNICALL
Java_org_cef_callback_CefAuthCallback_1N_N_1Cancel(JNIEnv* env,
                                                   jobject obj,
                                                   jlong self) {
  CefRefPtr<CefAuthCallback> callback = GetSelf(self);
  if (!callback)
    return;
  callback->Cancel();
  ClearSelf(env, obj);
}
