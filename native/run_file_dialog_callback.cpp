// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "run_file_dialog_callback.h"

#include "jni_scoped_helpers.h"
#include "jni_util.h"
#include "util.h"

RunFileDialogCallback::RunFileDialogCallback(JNIEnv* env, jobject jcallback)
    : handle_(env, jcallback) {}

void RunFileDialogCallback::OnFileDialogDismissed(
    const std::vector<CefString>& file_paths) {
  ScopedJNIEnv env;
  if (!env)
    return;

  ScopedJNIObjectLocal jfilePaths(env, NewJNIStringVector(env, file_paths));
  JNI_CALL_VOID_METHOD(env, handle_, "onFileDialogDismissed",
                       "(Ljava/util/Vector;)V", jfilePaths.get());
}
