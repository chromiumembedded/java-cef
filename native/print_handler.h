// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef CEF_TESTS_CEFCLIENT_CONTEXT_MENU_HANDLER_H_
#define CEF_TESTS_CEFCLIENT_CONTEXT_MENU_HANDLER_H_
#pragma once

#include <jni.h>
#include "include/cef_print_handler.h"

// PrintHandler implementation.
class PrintHandler : public CefPrintHandler {
 public:
  PrintHandler(JNIEnv* env, jobject handler);
  virtual ~PrintHandler();

  // CefPrintHandler methods
  virtual void OnPrintStart(CefRefPtr<CefBrowser> browser) OVERRIDE;

  virtual void OnPrintSettings(CefRefPtr<CefPrintSettings> settings,
                               bool get_defaults) OVERRIDE;

  virtual bool OnPrintDialog(bool has_selection,
                             CefRefPtr<CefPrintDialogCallback> callback) OVERRIDE;

  virtual bool OnPrintJob(const CefString& document_name,
                          const CefString& pdf_file_path,
                          CefRefPtr<CefPrintJobCallback> callback) OVERRIDE;

  virtual void OnPrintReset() OVERRIDE;

 protected:
  jobject jhandler_;

  // Include the default reference counting implementation.
  IMPLEMENT_REFCOUNTING(PrintHandler);
};

#endif  // CEF_TESTS_CEFCLIENT_CONTEXT_MENU_HANDLER_H_
