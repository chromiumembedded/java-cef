// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef JCEF_NATIVE_WEB_PLUGIN_UNSTABLE_CALLBACK_H_
#define JCEF_NATIVE_WEB_PLUGIN_UNSTABLE_CALLBACK_H_
#pragma once

#include <jni.h>

#include "include/cef_web_plugin.h"

#include "jni_scoped_helpers.h"

// WebPluginUnstableCallback implementation.
class WebPluginUnstableCallback : public CefWebPluginUnstableCallback {
 public:
  WebPluginUnstableCallback(JNIEnv* env, jobject jcallback);

  // WebPluginUnstableCallback methods
  virtual void IsUnstable(const CefString& path, bool unstable) override;

 protected:
  ScopedJNIObjectGlobal handle_;

  // Include the default reference counting implementation.
  IMPLEMENT_REFCOUNTING(WebPluginUnstableCallback);
};

#endif  // JCEF_NATIVE_WEB_PLUGIN_UNSTABLE_CALLBACK_H_
