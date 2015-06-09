// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "CefPrintDialogCallback_N.h"
#include "jni_util.h"
#include "include/cef_print_handler.h"

JNIEXPORT void JNICALL Java_org_cef_callback_CefPrintDialogCallback_1N_N_1Continue
  (JNIEnv *env, jobject obj, jobject jprintsettings) {
  CefRefPtr<CefPrintDialogCallback> callback =
      GetCefFromJNIObject<CefPrintDialogCallback>(env, obj,
          "CefPrintDialogCallback");
  if (!callback.get())
    return;

  CefRefPtr<CefPrintSettings> settings =
      GetCefFromJNIObject<CefPrintSettings>(env, jprintsettings,
          "CefPrintSettings");

  callback->Continue(settings);

  // Clear the reference added in PrintHandler::OnPrintDialog.
  SetCefForJNIObject<CefPrintDialogCallback>(env, obj, NULL,
      "CefPrintDialogCallback");
}

JNIEXPORT void JNICALL Java_org_cef_callback_CefPrintDialogCallback_1N_N_1Cancel
  (JNIEnv *env, jobject obj) {
  CefRefPtr<CefPrintDialogCallback> callback =
      GetCefFromJNIObject<CefPrintDialogCallback>(env, obj,
          "CefPrintDialogCallback");
  if (!callback.get())
    return;

  callback->Cancel();

  // Clear the reference added in PrintHandler::OnPrintDialog.
  SetCefForJNIObject<CefPrintDialogCallback>(env, obj, NULL,
      "CefPrintDialogCallback");
}
