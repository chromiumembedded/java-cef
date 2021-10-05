// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef JCEF_NATIVE_PRINT_HANDLER_H_
#define JCEF_NATIVE_PRINT_HANDLER_H_
#pragma once

#include <jni.h>

#include "include/cef_print_handler.h"

#include "jni_scoped_helpers.h"

// PrintHandler implementation.
class PrintHandler : public CefPrintHandler {
 public:
  PrintHandler(JNIEnv* env, jobject handler);

  // CefPrintHandler methods
  virtual void OnPrintStart(CefRefPtr<CefBrowser> browser) override;

  virtual void OnPrintSettings(CefRefPtr<CefBrowser> browser,
                               CefRefPtr<CefPrintSettings> settings,
                               bool get_defaults) override;

  virtual bool OnPrintDialog(
      CefRefPtr<CefBrowser> browser,
      bool has_selection,
      CefRefPtr<CefPrintDialogCallback> callback) override;

  virtual bool OnPrintJob(CefRefPtr<CefBrowser> browser,
                          const CefString& document_name,
                          const CefString& pdf_file_path,
                          CefRefPtr<CefPrintJobCallback> callback) override;

  virtual void OnPrintReset(CefRefPtr<CefBrowser> browser) override;

  virtual CefSize GetPdfPaperSize(CefRefPtr<CefBrowser> browser,
                                  int device_units_per_inch) override;

 protected:
  ScopedJNIObjectGlobal handle_;

  // Include the default reference counting implementation.
  IMPLEMENT_REFCOUNTING(PrintHandler);
};

#endif  // CEF_TESTS_CEFCLIENT_CONTEXT_MENU_HANDLER_H_
