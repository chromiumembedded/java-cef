// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef JCEF_NATIVE_RUN_FILE_DIALOG_CALLBACK_H_
#define JCEF_NATIVE_RUN_FILE_DIALOG_CALLBACK_H_
#pragma once

#include <jni.h>

#include "include/cef_browser.h"

#include "jni_scoped_helpers.h"

// RunFileDialogCallback implementation.
class RunFileDialogCallback : public CefRunFileDialogCallback {
 public:
  RunFileDialogCallback(JNIEnv* env, jobject jcallback);

  // RunFileDialogCallback methods
  virtual void OnFileDialogDismissed(
      const std::vector<CefString>& file_paths) override;

 protected:
  ScopedJNIObjectGlobal handle_;

  // Include the default reference counting implementation.
  IMPLEMENT_REFCOUNTING(RunFileDialogCallback);
};

#endif  // JCEF_NATIVE_RUN_FILE_DIALOG_CALLBACK_H_
