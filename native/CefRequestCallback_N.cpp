// Copyright (c) 2015 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "CefRequestCallback_N.h"
#include "include/cef_request_handler.h"
#include "jni_util.h"

JNIEXPORT void JNICALL Java_org_cef_callback_CefRequestCallback_1N_N_1Continue
  (JNIEnv *env, jobject obj, jboolean jallow) {
  CefRefPtr<CefRequestCallback> callback =
      GetCefFromJNIObject<CefRequestCallback>(env, obj, "CefRequestCallback");
  if (!callback.get())
    return;
  callback->Continue(jallow != JNI_FALSE);

  // Clear the reference added in RequestHandler.
  SetCefForJNIObject<CefRequestCallback>(env, obj, NULL, "CefRequestCallback");
}

JNIEXPORT void JNICALL Java_org_cef_callback_CefRequestCallback_1N_N_1Cancel
  (JNIEnv *env, jobject obj) {
  CefRefPtr<CefRequestCallback> callback =
      GetCefFromJNIObject<CefRequestCallback>(env, obj, "CefRequestCallback");
  if (!callback.get())
    return;
  callback->Cancel();

  // Clear the reference added in RequestHandler.
  SetCefForJNIObject<CefRequestCallback>(env, obj, NULL, "CefRequestCallback");
}
