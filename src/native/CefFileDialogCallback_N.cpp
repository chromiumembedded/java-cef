// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "CefFileDialogCallback_N.h"
#include "include/cef_dialog_handler.h"
#include "jni_util.h"

JNIEXPORT void JNICALL Java_org_cef_callback_CefFileDialogCallback_1N_N_1Continue
  (JNIEnv *env, jobject obj, jint selectedAcceptFilter, jobject jFilePaths) {
  CefRefPtr<CefFileDialogCallback> callback =
      GetCefFromJNIObject<CefFileDialogCallback>(env, obj, "CefFileDialogCallback");
  if (!callback.get())
    return;

  std::vector<CefString> filePaths;
  GetJNIStringVector(env, jFilePaths, filePaths);
  callback->Continue(selectedAcceptFilter, filePaths);

  // Clear the reference added in DialogHandler::OnFileDialog.
  SetCefForJNIObject<CefFileDialogCallback>(env, obj, NULL, "CefFileDialogCallback");
}

JNIEXPORT void JNICALL Java_org_cef_callback_CefFileDialogCallback_1N_N_1Cancel
  (JNIEnv *env, jobject obj) {
  CefRefPtr<CefFileDialogCallback> callback =
      GetCefFromJNIObject<CefFileDialogCallback>(env, obj, "CefFileDialogCallback");
  if (!callback.get())
    return;
  callback->Cancel();

  // Clear the reference added in DialogHandler::OnFileDialog.
  SetCefForJNIObject<CefFileDialogCallback>(env, obj, NULL, "CefFileDialogCallback");
}
