// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef CEF_TESTS_CEFCLIENT_CLIENT_APP_H_
#define CEF_TESTS_CEFCLIENT_CLIENT_APP_H_
#pragma once

#include <jni.h>
#include "browser_process_handler.h"
#include "include/cef_app.h"

// ClientApp implementation.
class ClientApp : public CefApp {
 public:
  explicit ClientApp(const std::string& module_dir,
                     const jobject app_handler);
  virtual ~ClientApp();

  // CefApp methods
  virtual void OnBeforeCommandLineProcessing(const CefString& process_type,
      CefRefPtr<CefCommandLine> command_line) OVERRIDE;

  // Additional method for JCEF
  bool HandleTerminate();

  virtual void OnRegisterCustomSchemes(CefRefPtr<CefSchemeRegistrar> registrar)
      OVERRIDE;

  virtual CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler()
      OVERRIDE;

  // Used to manage the lifespan of temporary files.
  static void registerTempFile(const std::string& tmpFile);
  static void eraseTempFiles();
 protected:
  std::string module_dir_;
  jobject app_handler_;
  CefRefPtr<BrowserProcessHandler> process_handler_;

  // Include the default reference counting implementation.
  IMPLEMENT_REFCOUNTING(ClientApp);
};

#endif  // CEF_TESTS_CEFCLIENT_CLIENT_APP_H_
