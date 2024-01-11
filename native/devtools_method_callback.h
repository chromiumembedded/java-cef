// Copyright (c) 2024 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef JCEF_NATIVE_DEVTOOLS_METHOD_CALLBACK_H_
#define JCEF_NATIVE_DEVTOOLS_METHOD_CALLBACK_H_
#pragma once

#include <jni.h>

#include "jni_scoped_helpers.h"

// Callback for CefBrowserHost::ExecuteDevToolsMethod. The methods of
// this class will be called on the browser process UI thread.
class DevToolsMethodCallback : public virtual CefBaseRefCounted {
 public:
  DevToolsMethodCallback(JNIEnv* env, jobject jcallback);

  // Method that will be called when the DevTools method has been executed.
  // |generatedMessageId| is the generated message ID, or 0 if an error
  // occurred.
  virtual void onComplete(int generatedMessageId);

 protected:
  ScopedJNIObjectGlobal handle_;

  // Include the default reference counting implementation.
  IMPLEMENT_REFCOUNTING(DevToolsMethodCallback);
};

#endif  // JCEF_NATIVE_DEVTOOLS_METHOD_CALLBACK_H_
