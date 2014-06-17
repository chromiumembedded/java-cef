// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "include/cef_app.h"
#include "include/wrapper/cef_message_router.h"

#if defined(OS_WIN)
#include <windows.h>
#endif

namespace {

// comparator to check if configuration values are the same
struct cmpCfg {
  bool operator() (const CefMessageRouterConfig& lValue,
                   const CefMessageRouterConfig& rValue) const {
    std::less<std::string> comp;
    return comp(lValue.js_query_function.ToString(),
                rValue.js_query_function.ToString());
  }
};

class CefHelperApp : public CefApp,
                     public CefRenderProcessHandler {
public:
  CefHelperApp() {}

  virtual CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler() OVERRIDE {
    return this;
  }

  virtual void OnRenderThreadCreated(CefRefPtr<CefListValue> extra_info) OVERRIDE {
    AutoLock lock_scope(this);
    for (size_t idx=0; idx < extra_info->GetSize(); idx++) {
      CefRefPtr<CefDictionaryValue> dict = extra_info->GetDictionary((int)idx);
      // Create the renderer-side router for query handling.
      CefMessageRouterConfig config;
      config.js_query_function = dict->GetString("js_query_function");
      config.js_cancel_function = dict->GetString("js_cancel_function");

      CefRefPtr<CefMessageRouterRendererSide> router =
          CefMessageRouterRendererSide::Create(config);
      message_router_.insert(std::make_pair(config, router));
    }
  }

  virtual void OnContextCreated(CefRefPtr<CefBrowser> browser,
                                CefRefPtr<CefFrame> frame,
                                CefRefPtr<CefV8Context> context) OVERRIDE {
    AutoLock lock_scope(this);
    std::map<CefMessageRouterConfig,
             CefRefPtr<CefMessageRouterRendererSide>,
             cmpCfg>::iterator iter;
    for (iter = message_router_.begin(); iter != message_router_.end(); iter++) {
      iter->second->OnContextCreated(browser,  frame, context);
    }
  }

  virtual void OnContextReleased(CefRefPtr<CefBrowser> browser,
                                 CefRefPtr<CefFrame> frame,
                                 CefRefPtr<CefV8Context> context) OVERRIDE {
    AutoLock lock_scope(this);
    std::map<CefMessageRouterConfig,
             CefRefPtr<CefMessageRouterRendererSide>,
             cmpCfg>::iterator iter;
    for (iter = message_router_.begin(); iter != message_router_.end(); iter++) {
      iter->second->OnContextReleased(browser, frame, context);
    }
  }

  virtual bool OnProcessMessageReceived(
      CefRefPtr<CefBrowser> browser,
      CefProcessId source_process,
      CefRefPtr<CefProcessMessage> message) OVERRIDE {
    AutoLock lock_scope(this);

    if (message->GetName() == "AddMessageRouter") {
      CefRefPtr<CefListValue> args = message->GetArgumentList();
      CefMessageRouterConfig config;
      config.js_query_function = args->GetString(0);
      config.js_cancel_function = args->GetString(1);

      // only add a new message router if it wasn't already created 
      if (message_router_.find(config) != message_router_.end()) {
        return true;
      }

      CefRefPtr<CefMessageRouterRendererSide> router =
          CefMessageRouterRendererSide::Create(config);
      message_router_.insert(std::make_pair(config, router));
      return true;

    } else if (message->GetName() == "RemoveMessageRouter") {
      CefRefPtr<CefListValue> args = message->GetArgumentList();
      CefMessageRouterConfig config;
      config.js_query_function = args->GetString(0);
      config.js_cancel_function = args->GetString(1);

      message_router_.erase(config);
      return true;
    }

    bool handled = false;
    std::map<CefMessageRouterConfig,
             CefRefPtr<CefMessageRouterRendererSide>,
             cmpCfg>::iterator iter;
    for (iter = message_router_.begin(); iter != message_router_.end(); iter++) {
      handled = iter->second->OnProcessMessageReceived(
          browser, source_process, message);
      if (handled)
        break;
    }
    return handled;
  }

 private:
  std::map<CefMessageRouterConfig,
           CefRefPtr<CefMessageRouterRendererSide>,
           cmpCfg> message_router_;

  IMPLEMENT_REFCOUNTING(CefHelperApp);
  IMPLEMENT_LOCKING(CefHelperApp);
};

}  // namespace

#if defined(OS_WIN)
int CALLBACK WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow) {
  CefMainArgs main_args(hInstance);
#else
int main(int argc, char* argv[]) {
  CefMainArgs main_args(argc, argv);
#endif

  CefRefPtr<CefHelperApp> app = new CefHelperApp();
  return CefExecuteProcess(main_args, app.get(), NULL);
}
