// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef JCEF_NATIVE_SCHEME_HANDLER_FACTORY_H_
#define JCEF_NATIVE_SCHEME_HANDLER_FACTORY_H_
#pragma once

#include <jni.h>
#include "include/cef_scheme.h"

// StringVisitor implementation.
class SchemeHandlerFactory : public CefSchemeHandlerFactory {
 public:
  SchemeHandlerFactory(JNIEnv* env, jobject jfactory);
  virtual ~SchemeHandlerFactory();

  // CefSchemeHandlerFactory methods
  virtual CefRefPtr<CefResourceHandler> Create(
      CefRefPtr<CefBrowser> browser,
      CefRefPtr<CefFrame> frame,
      const CefString& scheme_name,
      CefRefPtr<CefRequest> request) OVERRIDE;

 protected:
  jobject jfactory_;

  // Include the default reference counting implementation.
  IMPLEMENT_REFCOUNTING(SchemeHandlerFactory);
};

#endif  // JCEF_NATIVE_SCHEME_HANDLER_FACTORY_H_
