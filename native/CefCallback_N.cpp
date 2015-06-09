// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "CefCallback_N.h"
#include "include/cef_callback.h"
#include "jni_util.h"

JNIEXPORT void JNICALL Java_org_cef_callback_CefCallback_1N_N_1Continue
  (JNIEnv *env, jobject obj) {
  CefRefPtr<CefCallback> callback = GetCefFromJNIObject<CefCallback>(env, obj, "CefCallback");
  if (!callback)
    return;
  callback->Continue();
  SetCefForJNIObject<CefCallback>(env, obj, NULL, "CefCallback");
}

JNIEXPORT void JNICALL Java_org_cef_callback_CefCallback_1N_N_1Cancel
  (JNIEnv *env , jobject obj) {
  CefRefPtr<CefCallback> callback = GetCefFromJNIObject<CefCallback>(env, obj, "CefCallback");
  if (!callback)
    return;
  callback->Cancel();
  SetCefForJNIObject<CefCallback>(env, obj, NULL, "CefCallback");
}
