// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "CefJSDialogCallback_N.h"
#include "include/cef_jsdialog_handler.h"
#include "jni_util.h"

JNIEXPORT void JNICALL Java_org_cef_callback_CefJSDialogCallback_1N_N_1Continue
  (JNIEnv *env, jobject obj, jboolean jsuccess, jstring juser_input) {
  CefRefPtr<CefJSDialogCallback> jsDialogCallback =
      GetCefFromJNIObject<CefJSDialogCallback>(env, obj, "CefJSDialogCallback");
  if (!jsDialogCallback.get())
    return;
  jsDialogCallback->Continue((jsuccess != JNI_FALSE),
                             GetJNIString(env, juser_input));

  // Clear the reference added in JSDialogCallback::OnJSDialog and JSDialogCallback::OnBeforeUnloadDialog.
  SetCefForJNIObject<CefJSDialogCallback>(env, obj, NULL, "CefJSDialogCallback");
}
