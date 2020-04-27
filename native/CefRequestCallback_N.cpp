// Copyright (c) 2015 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "CefRequestCallback_N.h"
#include "include/cef_request_handler.h"
#include "jni_scoped_helpers.h"

namespace {

CefRefPtr<CefRequestCallback> GetSelf(jlong self) {
  return reinterpret_cast<CefRequestCallback*>(self);
}

void ClearSelf(JNIEnv* env, jobject obj) {
  // Clear the reference added in RequestHandler.
  SetCefForJNIObject<CefRequestCallback>(env, obj, NULL, "CefRequestCallback");
}

}  // namespace

JNIEXPORT void JNICALL
Java_org_cef_callback_CefRequestCallback_1N_N_1Continue(JNIEnv* env,
                                                        jobject obj,
                                                        jlong self,
                                                        jboolean jallow) {
  CefRefPtr<CefRequestCallback> callback = GetSelf(self);
  if (!callback)
    return;
  callback->Continue(jallow != JNI_FALSE);
  ClearSelf(env, obj);
}

JNIEXPORT void JNICALL
Java_org_cef_callback_CefRequestCallback_1N_N_1Cancel(JNIEnv* env,
                                                      jobject obj,
                                                      jlong self) {
  CefRefPtr<CefRequestCallback> callback = GetSelf(self);
  if (!callback)
    return;
  callback->Cancel();
  ClearSelf(env, obj);
}
