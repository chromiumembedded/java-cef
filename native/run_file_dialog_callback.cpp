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
    int selected_accept_filter,
    const std::vector<CefString>& file_paths) {
  if (!jcallback_)
    return;

  JNIEnv* env = GetJNIEnv();
  if (!env)
    return;
  jobject jfile_paths = NewJNIStringVector(env, file_paths);
  JNI_CALL_VOID_METHOD(env, jcallback_, "onFileDialogDismissed",
                       "(ILjava/util/Vector;)V", selected_accept_filter,
                       jfile_paths);
  env->DeleteLocalRef(jfile_paths);
}
