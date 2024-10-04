// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef JCEF_NATIVE_DOWNLOAD_HANDLER_H_
#define JCEF_NATIVE_DOWNLOAD_HANDLER_H_
#pragma once

#include <jni.h>
#include "include/cef_download_handler.h"

#include "jni_scoped_helpers.h"

// DownloadHandler implementation.
class DownloadHandler : public CefDownloadHandler {
 public:
  DownloadHandler(JNIEnv* env, jobject handler);

  // CefDownloadHandler methods
  virtual bool OnBeforeDownload(
      CefRefPtr<CefBrowser> browser,
      CefRefPtr<CefDownloadItem> download_item,
      const CefString& suggested_name,
      CefRefPtr<CefBeforeDownloadCallback> callback) override;

  virtual void OnDownloadUpdated(
      CefRefPtr<CefBrowser> browser,
      CefRefPtr<CefDownloadItem> download_item,
      CefRefPtr<CefDownloadItemCallback> callback) override;

 protected:
  ScopedJNIObjectGlobal handle_;

  // Include the default reference counting implementation.
  IMPLEMENT_REFCOUNTING(DownloadHandler);
};

#endif  // JCEF_NATIVE_DOWNLOAD_HANDLER_H_
