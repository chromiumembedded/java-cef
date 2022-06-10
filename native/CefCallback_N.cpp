// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "CefCallback_N.h"
#include "include/cef_callback.h"
#include "jni_scoped_helpers.h"

namespace {

CefRefPtr<CefCallback> GetSelf(jlong self) {
  return reinterpret_cast<CefCallback*>(self);
}

void ClearSelf(JNIEnv* env, jobject obj) {
  // Clear the reference added in ResourceHandler.
  SetCefForJNIObject<CefCallback>(env, obj, nullptr, "CefCallback");
}

}  // namespace

JNIEXPORT void JNICALL
Java_org_cef_callback_CefCallback_1N_N_1Continue(JNIEnv* env,
                                                 jobject obj,
                                                 jlong self) {
  CefRefPtr<CefCallback> callback = GetSelf(self);
  if (!callback)
    return;
  callback->Continue();
  ClearSelf(env, obj);
}

JNIEXPORT void JNICALL
Java_org_cef_callback_CefCallback_1N_N_1Cancel(JNIEnv* env,
                                               jobject obj,
                                               jlong self) {
  CefRefPtr<CefCallback> callback = GetSelf(self);
  if (!callback)
    return;
  callback->Cancel();
  ClearSelf(env, obj);
}
