// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "CefPrintJobCallback_N.h"
#include "jni_util.h"

#include "include/cef_print_handler.h"

JNIEXPORT void JNICALL Java_org_cef_callback_CefPrintJobCallback_1N_N_1Continue
  (JNIEnv *env, jobject obj) {
  CefRefPtr<CefPrintJobCallback> callback =
      GetCefFromJNIObject<CefPrintJobCallback>(env, obj, "CefPrintJobCallback");
  if (!callback.get())
    return;

  callback->Continue();

  // Clear the reference added in PrintHandler::OnPrintJob.
  SetCefForJNIObject<CefPrintJobCallback>(env, obj, NULL,
      "CefPrintJobCallback");
}
