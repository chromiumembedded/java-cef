// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef JCEF_NATIVE_COMPLETION_CALLBACK_H_
#define JCEF_NATIVE_COMPLETION_CALLBACK_H_
#pragma once

#include <jni.h>

#include "include/cef_callback.h"

#include "jni_scoped_helpers.h"

// CompletionCallback implementation.
class CompletionCallback : public CefCompletionCallback {
 public:
  CompletionCallback(JNIEnv* env, jobject jhandler);

  // CompletionCallback methods
  virtual void OnComplete() override;

 protected:
  ScopedJNIObjectGlobal handle_;

  // Include the default reference counting implementation.
  IMPLEMENT_REFCOUNTING(CompletionCallback);
};

#endif  // JCEF_NATIVE_COMPLETION_CALLBACK_H_
