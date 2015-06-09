// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "CefBeforeDownloadCallback_N.h"
#include "include/cef_download_handler.h"
#include "jni_util.h"

JNIEXPORT void JNICALL Java_org_cef_callback_CefBeforeDownloadCallback_1N_N_1Continue
  (JNIEnv *env, jobject obj, jstring jdownloadPath, jboolean jshowDialog) {
  CefRefPtr<CefBeforeDownloadCallback> callback =
      GetCefFromJNIObject<CefBeforeDownloadCallback>(env, obj, "CefBeforeDownloadCallback");
  if (!callback.get())
    return;

  callback->Continue(GetJNIString(env, jdownloadPath),
                     jshowDialog != JNI_FALSE);

  // Clear the reference added in DownloadHandler::OnBeforeDownload.
  SetCefForJNIObject<CefBeforeDownloadCallback>(env, obj, NULL, "CefBeforeDownloadCallback");
}
