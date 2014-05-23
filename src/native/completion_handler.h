// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef CEF_TESTS_CEFCLIENT_COMPLETION_HANDLER_VISITOR_H_
#define CEF_TESTS_CEFCLIENT_COMPLETION_HANDLER_VISITOR_H_
#pragma once

#include <jni.h>
#include "include/cef_callback.h"

// CompletionHandler implementation.
class CompletionHandler : public CefCompletionHandler {
 public:
  CompletionHandler(JNIEnv* env, jobject jhandler);
  virtual ~CompletionHandler();

  // CompletionHandler methods
  virtual void OnComplete() OVERRIDE;

 protected:
  jobject jhandler_;

  // Include the default reference counting implementation.
  IMPLEMENT_REFCOUNTING(CompletionHandler);
};

#endif  // CEF_TESTS_CEFCLIENT_COMPLETION_HANDLER_VISITOR_H_
