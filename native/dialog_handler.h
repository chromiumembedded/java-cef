// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef JCEF_NATIVE_DIALOG_HANDLER_H_
#define JCEF_NATIVE_DIALOG_HANDLER_H_
#pragma once

#include <jni.h>
#include "include/cef_dialog_handler.h"

#include "jni_scoped_helpers.h"

// DialogHandler implementation.
class DialogHandler : public CefDialogHandler {
 public:
  DialogHandler(JNIEnv* env, jobject handler);

  // CefDialogHandler methods
  virtual bool OnFileDialog(CefRefPtr<CefBrowser> browser,
                            FileDialogMode mode,
                            const CefString& title,
                            const CefString& default_file_path,
                            const std::vector<CefString>& accept_filters,
                            const std::vector<CefString>& accept_extensions,
                            const std::vector<CefString>& accept_descriptions,
                            CefRefPtr<CefFileDialogCallback> callback) override;

 protected:
  ScopedJNIObjectGlobal handle_;

  // Include the default reference counting implementation.
  IMPLEMENT_REFCOUNTING(DialogHandler);
};

#endif  // JCEF_NATIVE_DIALOG_HANDLER_H_
