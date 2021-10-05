// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef JCEF_NATIVE_STRING_VISITOR_H_
#define JCEF_NATIVE_STRING_VISITOR_H_
#pragma once

#include <jni.h>

#include "include/cef_string_visitor.h"

#include "jni_scoped_helpers.h"

// StringVisitor implementation.
class StringVisitor : public CefStringVisitor {
 public:
  StringVisitor(JNIEnv* env, jobject jvisitor);

  // StringVisitor methods
  virtual void Visit(const CefString& string) override;

 protected:
  ScopedJNIObjectGlobal handle_;

  // Include the default reference counting implementation.
  IMPLEMENT_REFCOUNTING(StringVisitor);
};

#endif  // JCEF_NATIVE_STRING_VISITOR_H_
