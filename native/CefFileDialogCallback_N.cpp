// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "CefFileDialogCallback_N.h"
#include "include/cef_dialog_handler.h"
#include "jni_scoped_helpers.h"
#include "jni_util.h"

namespace {

CefRefPtr<CefFileDialogCallback> GetSelf(jlong self) {
  return reinterpret_cast<CefFileDialogCallback*>(self);
}

void ClearSelf(JNIEnv* env, jobject obj) {
  // Clear the reference added in DialogHandler::OnFileDialog.
  SetCefForJNIObject<CefFileDialogCallback>(env, obj, nullptr,
                                            "CefFileDialogCallback");
}

}  // namespace

JNIEXPORT void JNICALL
Java_org_cef_callback_CefFileDialogCallback_1N_N_1Continue(JNIEnv* env,
                                                           jobject obj,
                                                           jlong self,
                                                           jobject jFilePaths) {
  CefRefPtr<CefFileDialogCallback> callback = GetSelf(self);
  if (!callback)
    return;

  std::vector<CefString> filePaths;
  GetJNIStringVector(env, jFilePaths, filePaths);
  callback->Continue(filePaths);

  ClearSelf(env, obj);
}

JNIEXPORT void JNICALL
Java_org_cef_callback_CefFileDialogCallback_1N_N_1Cancel(JNIEnv* env,
                                                         jobject obj,
                                                         jlong self) {
  CefRefPtr<CefFileDialogCallback> callback = GetSelf(self);
  if (!callback)
    return;
  callback->Cancel();
  ClearSelf(env, obj);
}
