// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef JCEF_NATIVE_CLIENT_APP_H_
#define JCEF_NATIVE_CLIENT_APP_H_
#pragma once

#include <jni.h>

#include "include/cef_app.h"

#include "browser_process_handler.h"
#include "jni_scoped_helpers.h"

// ClientApp implementation.
class ClientApp : public CefApp {
 public:
  ClientApp(const std::string& cache_path,
            JNIEnv* env,
            const jobject app_handler);

  // CefApp methods:
  void OnBeforeCommandLineProcessing(
      const CefString& process_type,
      CefRefPtr<CefCommandLine> command_line) override;
  void OnRegisterCustomSchemes(
      CefRawPtr<CefSchemeRegistrar> registrar) override;
  CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler() override;

#if defined(OS_MACOSX)
  // Used to continue termination handling in Java.
  bool HandleTerminate();
#endif

  // Used to manage the lifespan of temporary files.
  static void registerTempFile(const std::string& tmpFile);
  static void eraseTempFiles();

 protected:
  const std::string cache_path_;
  ScopedJNIObjectGlobal handle_;
  CefRefPtr<BrowserProcessHandler> process_handler_;

  // Include the default reference counting implementation.
  IMPLEMENT_REFCOUNTING(ClientApp);
};

#endif  // JCEF_NATIVE_CLIENT_APP_H_
