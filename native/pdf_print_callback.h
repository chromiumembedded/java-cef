// Copyright (c) 2017 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef JCEF_NATIVE_PDF_PRINT_CALLBACK_H_
#define JCEF_NATIVE_PDF_PRINT_CALLBACK_H_
#pragma once

#include <jni.h>

#include "include/cef_browser.h"

#include "jni_scoped_helpers.h"

// PdfPrintCallback implementation.
class PdfPrintCallback : public CefPdfPrintCallback {
 public:
  PdfPrintCallback(JNIEnv* env, jobject jcallback);

  // PdfPrintCallback methods
  virtual void OnPdfPrintFinished(const CefString& path, bool ok) override;

 protected:
  ScopedJNIObjectGlobal handle_;

  // Include the default reference counting implementation.
  IMPLEMENT_REFCOUNTING(PdfPrintCallback);
};

#endif  // JCEF_NATIVE_PDF_PRINT_CALLBACK_H_
