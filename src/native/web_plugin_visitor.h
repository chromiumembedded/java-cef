// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef JCEF_NATIVE_WEB_PLUGIN_INFO_VISITOR_H_
#define JCEF_NATIVE_WEB_PLUGIN_INFO_VISITOR_H_
#pragma once

#include <jni.h>
#include "include/cef_web_plugin.h"

// WebPluginInfoVisitor implementation.
class WebPluginInfoVisitor : public CefWebPluginInfoVisitor {
 public:
  WebPluginInfoVisitor(JNIEnv* env, jobject jvisitor);
  virtual ~WebPluginInfoVisitor();

  // WebPluginInfoVisitor methods
  virtual bool Visit(CefRefPtr<CefWebPluginInfo> info,
                     int count,
                     int total) OVERRIDE;

 protected:
  jobject jvisitor_;

  // Include the default reference counting implementation.
  IMPLEMENT_REFCOUNTING(WebPluginInfoVisitor);
};

#endif  // JCEF_NATIVE_WEB_PLUGIN_INFO_VISITOR_H_
