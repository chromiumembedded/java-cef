// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "CefPrintDialogCallback_N.h"
#include "include/cef_print_handler.h"
#include "jni_scoped_helpers.h"

namespace {

CefRefPtr<CefPrintDialogCallback> GetSelf(jlong self) {
  return reinterpret_cast<CefPrintDialogCallback*>(self);
}

void ClearSelf(JNIEnv* env, jobject obj) {
  // Clear the reference added in PrintHandler::OnPrintDialog.
  SetCefForJNIObject<CefPrintDialogCallback>(env, obj, nullptr,
                                             "CefPrintDialogCallback");
}

}  // namespace

JNIEXPORT void JNICALL
Java_org_cef_callback_CefPrintDialogCallback_1N_N_1Continue(
    JNIEnv* env,
    jobject obj,
    jlong self,
    jobject jprintsettings) {
  CefRefPtr<CefPrintDialogCallback> callback = GetSelf(self);
  if (!callback)
    return;

  CefRefPtr<CefPrintSettings> settings = GetCefFromJNIObject<CefPrintSettings>(
      env, jprintsettings, "CefPrintSettings");
  if (settings) {
    callback->Continue(settings);
  } else {
    callback->Cancel();
  }
  ClearSelf(env, obj);
}

JNIEXPORT void JNICALL
Java_org_cef_callback_CefPrintDialogCallback_1N_N_1Cancel(JNIEnv* env,
                                                          jobject obj,
                                                          jlong self) {
  CefRefPtr<CefPrintDialogCallback> callback = GetSelf(self);
  if (!callback)
    return;
  callback->Cancel();
  ClearSelf(env, obj);
}
