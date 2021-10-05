// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef JCEF_NATIVE_JSDIALOG_HANDLER_H_
#define JCEF_NATIVE_JSDIALOG_HANDLER_H_
#pragma once

#include <jni.h>
#include "include/cef_jsdialog_handler.h"

#include "jni_scoped_helpers.h"

// JSDialogHandler implementation.
class JSDialogHandler : public CefJSDialogHandler {
 public:
  JSDialogHandler(JNIEnv* env, jobject handler);

  // CefJSDialogHandler methods
  virtual bool OnJSDialog(CefRefPtr<CefBrowser> browser,
                          const CefString& origin_url,
                          JSDialogType dialog_type,
                          const CefString& message_text,
                          const CefString& default_prompt_text,
                          CefRefPtr<CefJSDialogCallback> callback,
                          bool& suppress_message) override;

  virtual bool OnBeforeUnloadDialog(
      CefRefPtr<CefBrowser> browser,
      const CefString& message_text,
      bool is_reload,
      CefRefPtr<CefJSDialogCallback> callback) override;

  virtual void OnResetDialogState(CefRefPtr<CefBrowser> browser) override;
  virtual void OnDialogClosed(CefRefPtr<CefBrowser> browser) override;

 protected:
  ScopedJNIObjectGlobal handle_;

  // Include the default reference counting implementation.
  IMPLEMENT_REFCOUNTING(JSDialogHandler);
};

#endif  // JCEF_NATIVE_JSDIALOG_HANDLER_H_
