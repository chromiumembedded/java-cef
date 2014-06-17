// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "CefApp.h"

#include <string>

#include "include/cef_app.h"
#include "include/cef_browser.h"
#include "include/cef_path_util.h"
#include "browser_process_handler.h"
#include "client_handler.h"
#include "render_handler.h"
#include "scheme_handler_factory.h"
#include "jni_util.h"
#include "util.h"

#if defined(OS_LINUX)
#include <gdk/gdkx.h>
#endif

#if defined(OS_MACOSX)
#include "util_mac.h"
#endif

namespace {

class ClientApp : public CefApp {
 public:
  explicit ClientApp(const std::string& module_dir,
                     const jobject app_handler)
      : module_dir_(module_dir), app_handler_(NULL) {
    JNIEnv *env = GetJNIEnv();
    if (env)
      app_handler_ = env->NewGlobalRef(app_handler);
    process_handler_ = new BrowserProcessHandler(app_handler_);
  }

  virtual ~ClientApp() {
    if (!app_handler_)
      return;
    BEGIN_ENV(env)
    env->DeleteGlobalRef(app_handler_);
    END_ENV(env)
  }

  virtual void OnBeforeCommandLineProcessing(
      const CefString& process_type,
      CefRefPtr<CefCommandLine> command_line) OVERRIDE {
    // If the java code has registered an AppHandler, we'll forward
    // the commandline processing to it before we append the essential
    // switches "locale_pak" and "use-core-animation".
    if (app_handler_ != NULL && process_type.empty()) {
      BEGIN_ENV(env)
      jstring jprocess_type = NewJNIString(env, process_type);
      jobject jcommand_line = NewJNIObject(env, "org/cef/callback/CefCommandLine_N");
      if (jcommand_line != NULL) {
        SetCefForJNIObject(env, jcommand_line, command_line.get(), "CefCommandLine");
        JNI_CALL_VOID_METHOD(env,
                             app_handler_,
                             "onBeforeCommandLineProcessing",
                             "(Ljava/lang/String;Lorg/cef/callback/CefCommandLine;)V",
                             jprocess_type,
                             jcommand_line);
        SetCefForJNIObject<CefCommandLine>(env, jcommand_line, NULL, "CefCommandLine");
      }
      END_ENV(env)
    }

    if (process_type.empty()) {
#if defined(OS_MACOSX)
      // Specify a path for the locale.pak file because CEF will fail to locate
      // it based on the app bundle structure.
      const std::string& locale_path = util_mac::GetAbsPath(
          module_dir_ + "/../Frameworks/Chromium Embedded Framework.framework/"
                       "Resources/en.lproj/locale.pak");
      command_line->AppendSwitchWithValue("locale_pak", locale_path);
      // If windowed rendering is used, we need the browser window as CALayer
      // due Java7 is CALayer based instead of NSLayer based.
      command_line->AppendSwitch("use-core-animation");
#endif  // defined(OS_MACOSX)
    }
  }

  virtual void OnRegisterCustomSchemes(CefRefPtr<CefSchemeRegistrar> registrar) OVERRIDE {
    if (!app_handler_)
      return;

    BEGIN_ENV(env)
    jobject jregistrar = NewJNIObject(env, "org/cef/callback/CefSchemeRegistrar_N");
    if (jregistrar != NULL) {
      SetCefForJNIObject(env, jregistrar, registrar.get(), "CefSchemeRegistrar");
      JNI_CALL_VOID_METHOD(env,
                           app_handler_,
                           "onRegisterCustomSchemes",
                           "(Lorg/cef/callback/CefSchemeRegistrar;)V",
                           jregistrar);
      SetCefForJNIObject<CefSchemeRegistrar>(env, jregistrar, NULL, "CefSchemeRegistrar");
    }
    END_ENV(env)
  }

  virtual CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler() OVERRIDE {
    return process_handler_.get();
  }

 private:
  std::string module_dir_;
  jobject app_handler_;
  CefRefPtr<BrowserProcessHandler> process_handler_;

  IMPLEMENT_REFCOUNTING(ClientApp);
};

std::string GetHelperPath(const std::string& module_dir) {
#if defined(OS_MACOSX)
  return util_mac::GetAbsPath(
      module_dir + "/../Frameworks/jcef Helper.app/Contents/MacOS/jcef Helper");
#elif defined(OS_WIN)
  return module_dir + "\\jcef_helper.exe";
#elif defined(OS_LINUX)
  return module_dir + "/jcef_helper";
#endif
}

}  // namespace


JNIEXPORT jboolean JNICALL Java_org_cef_CefApp_N_1Initialize
  (JNIEnv *env, jobject c, jstring argPathToJavaDLL, jobject appHandler) {
  JavaVM* jvm;
  jint rs = env->GetJavaVM(&jvm);
  if (rs != JNI_OK) {
    ASSERT(false);  // Not reached.
    return false;
  }
  SetJVM(jvm);

#if defined(OS_WIN)
  CefMainArgs main_args(::GetModuleHandle(NULL));
#else
  CefMainArgs main_args(0, NULL);
#endif

  const std::string& module_dir = GetJNIString(env, argPathToJavaDLL);

  CefSettings settings;
  CefString(&settings.browser_subprocess_path) = GetHelperPath(module_dir);

  settings.no_sandbox = true;

#if defined(OS_LINUX)
  CefString(&settings.resources_dir_path) = module_dir;
  CefString(&settings.locales_dir_path) = module_dir + "/locales";
#endif

  CefRefPtr<ClientApp> client_app(new ClientApp(module_dir, appHandler));
#if defined(OS_MACOSX)
  return util_mac::CefInitializeOnMainThread(main_args, settings,
                                              client_app.get()) ?
      JNI_TRUE : JNI_FALSE;
#endif
  return CefInitialize(main_args, settings, client_app.get(), NULL) ?
      JNI_TRUE : JNI_FALSE;
}

JNIEXPORT void JNICALL Java_org_cef_CefApp_N_1Shutdown
  (JNIEnv *env, jobject) {
#if defined(OS_MACOSX)
  util_mac::CefQuitMessageLoopOnMainThread();
#else
  CefShutdown();
#endif
}

JNIEXPORT void JNICALL Java_org_cef_CefApp_N_1DoMessageLoopWork
  (JNIEnv *env, jobject) {
  CefDoMessageLoopWork();
}

JNIEXPORT jboolean JNICALL Java_org_cef_CefApp_N_1RegisterSchemeHandlerFactory
  (JNIEnv *env, jobject, jstring jSchemeName, jstring jDomainName, jobject jFactory) {
  if (!jFactory)
    return JNI_FALSE;

  CefRefPtr<SchemeHandlerFactory> factory = new SchemeHandlerFactory(env, jFactory);
  if (!factory)
    return JNI_FALSE;

  bool result = CefRegisterSchemeHandlerFactory(GetJNIString(env, jSchemeName),
                                                GetJNIString(env, jDomainName),
                                                factory.get());
  return result ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jboolean JNICALL Java_org_cef_CefApp_N_1ClearSchemeHandlerFactories
  (JNIEnv *, jobject) {
  return CefClearSchemeHandlerFactories() ? JNI_TRUE : JNI_FALSE;
}
