// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include <fstream>

#include "include/cef_app.h"
#include "include/wrapper/cef_message_router.h"
#include "util.h"

#if defined(OS_MACOSX)
// When generating projects with CMake the CEF_USE_SANDBOX value will be defined
// automatically. Pass -DUSE_SANDBOX=OFF to the CMake command-line to disable
// use of the sandbox.
#if defined(CEF_USE_SANDBOX)
#include "include/cef_sandbox_mac.h"
#endif

#include "include/wrapper/cef_library_loader.h"
#endif  // defined(OS_MACOSX)

#if defined(OS_WIN)
#include <windows.h>
#endif

namespace {

// comparator to check if configuration values are the same
struct cmpCfg {
  bool operator()(const CefMessageRouterConfig& lValue,
                  const CefMessageRouterConfig& rValue) const {
    std::less<std::string> comp;
    return comp(lValue.js_query_function.ToString(),
                rValue.js_query_function.ToString());
  }
};

class CefHelperApp : public CefApp, public CefRenderProcessHandler {
 public:
  CefHelperApp() {}

  void OnRegisterCustomSchemes(
      CefRawPtr<CefSchemeRegistrar> registrar) override {
    std::fstream fStream;
    std::string fName = util::GetTempFileName("scheme", true);
    char schemeName[512] = "";
    int options;

    fStream.open(fName.c_str(), std::fstream::in);
    while (fStream.is_open() && !fStream.eof()) {
      fStream.getline(schemeName, 512, ',');
      if (strlen(schemeName) == 0)
        break;

      fStream >> options;

      registrar->AddCustomScheme(schemeName, options);
    }
    fStream.close();
  }

  virtual CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler()
      override {
    return this;
  }

  void OnBrowserCreated(CefRefPtr<CefBrowser> browser,
                        CefRefPtr<CefDictionaryValue> extra_info) override {
    if (!extra_info) {
      return;
    }
    auto router_configs = extra_info->GetList("router_configs");
    if (router_configs) {
      // Configuration from BrowserProcessHandler::GetMessageRouterConfigs.
      for (size_t idx = 0; idx < router_configs->GetSize(); idx++) {
        CefRefPtr<CefDictionaryValue> dict =
            router_configs->GetDictionary((int)idx);
        // Create the renderer-side router for query handling.
        CefMessageRouterConfig config;
        config.js_query_function = dict->GetString("js_query_function");
        config.js_cancel_function = dict->GetString("js_cancel_function");

        CefRefPtr<CefMessageRouterRendererSide> router =
            CefMessageRouterRendererSide::Create(config);
        message_router_.insert(std::make_pair(config, router));
      }
    }
  }

  void OnContextCreated(CefRefPtr<CefBrowser> browser,
                        CefRefPtr<CefFrame> frame,
                        CefRefPtr<CefV8Context> context) override {
    std::map<CefMessageRouterConfig, CefRefPtr<CefMessageRouterRendererSide>,
             cmpCfg>::iterator iter;
    for (iter = message_router_.begin(); iter != message_router_.end();
         iter++) {
      iter->second->OnContextCreated(browser, frame, context);
    }
  }

  void OnContextReleased(CefRefPtr<CefBrowser> browser,
                         CefRefPtr<CefFrame> frame,
                         CefRefPtr<CefV8Context> context) override {
    std::map<CefMessageRouterConfig, CefRefPtr<CefMessageRouterRendererSide>,
             cmpCfg>::iterator iter;
    for (iter = message_router_.begin(); iter != message_router_.end();
         iter++) {
      iter->second->OnContextReleased(browser, frame, context);
    }
  }

  bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
                                CefRefPtr<CefFrame> frame,
                                CefProcessId source_process,
                                CefRefPtr<CefProcessMessage> message) override {
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
    std::map<CefMessageRouterConfig, CefRefPtr<CefMessageRouterRendererSide>,
             cmpCfg>::iterator iter;
    for (iter = message_router_.begin(); iter != message_router_.end();
         iter++) {
      handled = iter->second->OnProcessMessageReceived(browser, frame,
                                                       source_process, message);
      if (handled)
        break;
    }
    return handled;
  }

 private:
  std::map<CefMessageRouterConfig,
           CefRefPtr<CefMessageRouterRendererSide>,
           cmpCfg>
      message_router_;

  IMPLEMENT_REFCOUNTING(CefHelperApp);
};

}  // namespace

#if defined(OS_WIN)
int CALLBACK WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpCmdLine,
                     int nCmdShow) {
  CefMainArgs main_args(hInstance);
#else  // !defined(OS_WIN)
int main(int argc, char* argv[]) {
#if defined(OS_MACOSX)
#if defined(CEF_USE_SANDBOX)
  // Initialize the macOS sandbox for this helper process.
  CefScopedSandboxContext sandbox_context;
  if (!sandbox_context.Initialize(argc, argv))
    return 1;
#endif  // defined(CEF_USE_SANDBOX)

  // Check for the path on the command-line.
  std::string framework_path;
  const std::string switchPrefix = "--framework-dir-path=";
  for (int i = 0; i < argc; ++i) {
    std::string arg = argv[i];
    if (arg.find(switchPrefix) == 0) {
      framework_path = arg.substr(switchPrefix.length());
      break;
    }
  }

  // Load the CEF framework library at runtime instead of linking directly
  // as required by the macOS sandbox implementation.
  CefScopedLibraryLoader library_loader;
  if (!framework_path.empty()) {
    framework_path += "/Chromium Embedded Framework";
    if (!cef_load_library(framework_path.c_str()))
      return 1;
  } else if (!library_loader.LoadInHelper()) {
    return 1;
  }
#endif  // defined(OS_MACOSX)

  CefMainArgs main_args(argc, argv);
#endif  // !defined(OS_WIN)

  CefRefPtr<CefHelperApp> app = new CefHelperApp();
  const int result = CefExecuteProcess(main_args, app.get(), nullptr);

#if defined(OS_MACOSX)
  if (!framework_path.empty())
    cef_unload_library();
#endif

  return result;
}
