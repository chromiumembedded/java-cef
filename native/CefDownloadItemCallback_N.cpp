// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "CefDownloadItemCallback_N.h"
#include "include/cef_download_handler.h"
#include "jni_scoped_helpers.h"

namespace {

CefRefPtr<CefDownloadItemCallback> GetSelf(jlong self) {
  return reinterpret_cast<CefDownloadItemCallback*>(self);
}

void ClearSelf(JNIEnv* env, jobject obj) {
  // Clear the reference added in DownloadHandler::OnDownloadUpdated.
  SetCefForJNIObject<CefDownloadItemCallback>(env, obj, nullptr,
                                              "CefDownloadItemCallback");
}

}  // namespace

JNIEXPORT void JNICALL
Java_org_cef_callback_CefDownloadItemCallback_1N_N_1Cancel(JNIEnv* env,
                                                           jobject obj,
                                                           jlong self) {
  CefRefPtr<CefDownloadItemCallback> callback = GetSelf(self);
  if (!callback)
    return;
  callback->Cancel();
  ClearSelf(env, obj);
}

JNIEXPORT void JNICALL
Java_org_cef_callback_CefDownloadItemCallback_1N_N_1Pause(JNIEnv* env,
                                                          jobject obj,
                                                          jlong self) {
  CefRefPtr<CefDownloadItemCallback> callback = GetSelf(self);
  if (!callback)
    return;
  callback->Pause();
}

JNIEXPORT void JNICALL
Java_org_cef_callback_CefDownloadItemCallback_1N_N_1Resume(JNIEnv* env,
                                                           jobject obj,
                                                           jlong self) {
  CefRefPtr<CefDownloadItemCallback> callback = GetSelf(self);
  if (!callback)
    return;
  callback->Resume();
}

JNIEXPORT void JNICALL
Java_org_cef_callback_CefDownloadItemCallback_1N_N_1Dispose(JNIEnv* env,
                                                            jobject obj,
                                                            jlong self) {
  CefRefPtr<CefDownloadItemCallback> callback = GetSelf(self);
  if (!callback)
    return;
  // Intentionally not executing any callback methods here.
  ClearSelf(env, obj);
}
