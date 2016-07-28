// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef JCEF_NATIVE_BROWSER_PROCESS_HANDLER_H_
#define JCEF_NATIVE_BROWSER_PROCESS_HANDLER_H_
#pragma once

#include <set>
#include <jni.h>
#include "include/base/cef_lock.h"
#include "include/cef_browser_process_handler.h"
#include "include/wrapper/cef_message_router.h"

// comparator to check if configuration values are the same
struct cmpCfg {
  bool operator() (const CefMessageRouterConfig& lValue,
                   const CefMessageRouterConfig& rValue) const {
    std::less<std::string> comp;
    return comp(lValue.js_query_function.ToString(),
                rValue.js_query_function.ToString());
  }
};

// BrowserProcessHandler implementation.
class BrowserProcessHandler : public CefBrowserProcessHandler {
 public:
  BrowserProcessHandler(jobject app_handler);
  virtual ~BrowserProcessHandler();

  virtual void OnContextInitialized() OVERRIDE;

  virtual void OnRenderProcessThreadCreated(
      CefRefPtr<CefListValue> extra_info) OVERRIDE;

  virtual CefRefPtr<CefPrintHandler> GetPrintHandler() OVERRIDE;

  virtual void OnScheduleMessagePumpWork(int64 delay_ms) OVERRIDE;

  static void AddMessageRouterConfig(const CefMessageRouterConfig& cfg);  
  static void RemoveMessageRouterConfig(const CefMessageRouterConfig& cfg);
 protected:
  jobject app_handler_;
  static std::set<CefMessageRouterConfig, cmpCfg> router_cfg_;
  static base::Lock router_cfg_lock_;

  // Include the default reference counting implementation.
  IMPLEMENT_REFCOUNTING(BrowserProcessHandler);
};

#endif  // JCEF_NATIVE_BROWSER_PROCESS_HANDLER_H_
