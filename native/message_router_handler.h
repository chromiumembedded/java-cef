// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef JCEF_NATIVE_MESSAGE_ROUTER_HANDLER_H_
#define JCEF_NATIVE_MESSAGE_ROUTER_HANDLER_H_
#pragma once

#include <jni.h>

#include "include/wrapper/cef_message_router.h"

#include "jni_scoped_helpers.h"

// MessageRouterHandler implementation.
class MessageRouterHandler : public CefMessageRouterBrowserSide::Handler,
                             public CefBaseRefCounted {
 public:
  MessageRouterHandler(JNIEnv* env, jobject handler);

  // CefMessageRouterHandler methods
  virtual bool OnQuery(CefRefPtr<CefBrowser> browser,
                       CefRefPtr<CefFrame> frame,
                       int64_t query_id,
                       const CefString& request,
                       bool persistent,
                       CefRefPtr<Callback> callback) override;
  virtual void OnQueryCanceled(CefRefPtr<CefBrowser> browser,
                               CefRefPtr<CefFrame> frame,
                               int64_t query_id) override;

 protected:
  ScopedJNIObjectGlobal handle_;

  // Include the default reference counting implementation.
  IMPLEMENT_REFCOUNTING(MessageRouterHandler);
};

#endif  // JCEF_NATIVE_MESSAGE_ROUTER_HANDLER_H_
