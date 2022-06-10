// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "CefJSDialogCallback_N.h"
#include "include/cef_jsdialog_handler.h"
#include "jni_scoped_helpers.h"
#include "jni_util.h"

namespace {

CefRefPtr<CefJSDialogCallback> GetSelf(jlong self) {
  return reinterpret_cast<CefJSDialogCallback*>(self);
}

void ClearSelf(JNIEnv* env, jobject obj) {
  // Clear the reference added in JSDialogHandler::OnJSDialog and
  // JSDialogHandler::OnBeforeUnloadDialog.
  SetCefForJNIObject<CefJSDialogCallback>(env, obj, nullptr,
                                          "CefJSDialogCallback");
}

}  // namespace

JNIEXPORT void JNICALL
Java_org_cef_callback_CefJSDialogCallback_1N_N_1Continue(JNIEnv* env,
                                                         jobject obj,
                                                         jlong self,
                                                         jboolean jsuccess,
                                                         jstring juser_input) {
  CefRefPtr<CefJSDialogCallback> callback = GetSelf(self);
  if (!callback)
    return;
  callback->Continue((jsuccess != JNI_FALSE), GetJNIString(env, juser_input));
  ClearSelf(env, obj);
}
