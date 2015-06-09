// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "CefDownloadItemCallback_N.h"
#include "include/cef_download_handler.h"
#include "jni_util.h"

JNIEXPORT void JNICALL Java_org_cef_callback_CefDownloadItemCallback_1N_N_1Cancel
  (JNIEnv *env, jobject obj) {
  CefRefPtr<CefDownloadItemCallback> callback =
      GetCefFromJNIObject<CefDownloadItemCallback>(env, obj, "CefDownloadItemCallback");
  if (!callback.get())
    return;
  callback->Cancel();

  // Clear the reference added in DownloadHandler::OnDownloadUpdated.
  SetCefForJNIObject<CefBeforeDownloadCallback>(env, obj, NULL, "CefBeforeDownloadCallback");
}
