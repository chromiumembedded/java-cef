// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "run_file_dialog_callback.h"

#include "jni_util.h"
#include "util.h"

RunFileDialogCallback::RunFileDialogCallback(JNIEnv* env, jobject jcallback) {
  jcallback_ = env->NewGlobalRef(jcallback);
}

RunFileDialogCallback::~RunFileDialogCallback() {
  JNIEnv* env = GetJNIEnv();
  env->DeleteGlobalRef(jcallback_);
}

void RunFileDialogCallback::OnFileDialogDismissed(
      CefRefPtr<CefBrowserHost> browser_host,
      const std::vector<CefString>& file_paths) {
  if (!jcallback_)
    return;

  JNIEnv* env = GetJNIEnv();
  if (!env)
    return;
  JNI_CALL_VOID_METHOD(env, jcallback_,
                       "onFileDialogDismissed",
                       "(Lorg/cef/browser/CefBrowser;Ljava/util/Vector;)V",
                       GetJNIBrowser(browser_host->GetBrowser()),
                       NewJNIStringVector(env, file_paths));
}
