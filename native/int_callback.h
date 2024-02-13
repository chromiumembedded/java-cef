// Copyright (c) 2024 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef JCEF_NATIVE_INT_CALLBACK_H_
#define JCEF_NATIVE_INT_CALLBACK_H_
#pragma once

#include <jni.h>

#include "jni_scoped_helpers.h"

// Callback for returning int primatives. The methods of
// this class will be called on the browser process UI thread.
class IntCallback : public virtual CefBaseRefCounted {
 public:
  IntCallback(JNIEnv* env, jobject jcallback);

  virtual void onComplete(int value);

 protected:
  ScopedJNIObjectGlobal handle_;

  // Include the default reference counting implementation.
  IMPLEMENT_REFCOUNTING(IntCallback);
};

#endif  // JCEF_NATIVE_INT_CALLBACK_H_
