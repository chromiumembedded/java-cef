// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "CefContext.h"

#include <string>

#include "include/cef_app.h"
#include "include/cef_browser.h"
#include "include/cef_path_util.h"
#include "client_handler.h"
#include "jni_util.h"
#include "util.h"

#if defined(OS_LINUX)
#include <gdk/gdkx.h>
#endif

#if defined(OS_MACOSX)
#include "util_mac.h"
#endif

namespace {

// TODO(jcef): We may want to support both windowed and osr use cases in 
// the same application, in which case we shouldn't use a global variable.
static bool g_use_osr = false;

class ClientApp : public CefApp {
 public:
  explicit ClientApp(const std::string& module_dir)
      : module_dir_(module_dir) {
  }

#if defined(OS_MACOSX)
  virtual void OnBeforeCommandLineProcessing(
      const CefString& process_type,
      CefRefPtr<CefCommandLine> command_line) OVERRIDE {
    if (process_type.empty()) {
      // Specify a path for the locale.pak file because CEF will fail to locate
      // it based on the app bundle structure.
      const std::string& locale_path = util_mac::GetAbsPath(
          module_dir_ + "/../Frameworks/Chromium Embedded Framework.framework/"
                       "Resources/en.lproj/locale.pak");
      command_line->AppendSwitchWithValue("locale_pak", locale_path);
      // If windowed rendering is used, we need the browser window as CALayer
      // due Java7 is CALayer based instead of NSLayer based.
      if (!g_use_osr)
        command_line->AppendSwitch("use-core-animation");
    }
  }
#endif  // defined(OS_MACOSX)

 private:
  std::string module_dir_;

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


JNIEXPORT jboolean JNICALL Java_org_cef_CefContext_N_1Initialize
  (JNIEnv *env, jclass c, jstring argPathToJavaDLL, jstring cachePath,
   jboolean osrEnabled) {
  JavaVM* jvm;
  jint rs = env->GetJavaVM(&jvm);
  if (rs != JNI_OK) {
    ASSERT(false);  // Not reached.
    return false;
  }
  SetJVM(jvm);

  // Keep information if offscreen or windowed rendering is used.
  g_use_osr = (osrEnabled == JNI_TRUE);

#if defined(OS_WIN)
  CefMainArgs main_args(::GetModuleHandle(NULL));
#else
  CefMainArgs main_args(0, NULL);
#endif

  const std::string& module_dir = GetJNIString(env, argPathToJavaDLL);

  CefSettings settings;
  CefString(&settings.browser_subprocess_path) = GetHelperPath(module_dir);

#if defined(OS_LINUX)
  CefString(&settings.resources_dir_path) = module_dir;
  CefString(&settings.locales_dir_path) = module_dir + "/locales";
#endif

  CefRefPtr<ClientApp> client_app(new ClientApp(module_dir));
#if defined(OS_MACOSX)
  if (!g_use_osr)
    return util_mac::CefInitializeOnMainThread(main_args, settings, client_app.get()) ? JNI_TRUE : JNI_FALSE;
#endif
  return CefInitialize(main_args, settings, client_app.get()) ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT void JNICALL Java_org_cef_CefContext_N_1Shutdown
  (JNIEnv *env, jclass) {
#if defined(OS_MACOSX)
  if (!g_use_osr) {
    util_mac::CefQuitMessageLoopOnMainThread();
    return;
  }
#endif
  CefShutdown();
}

JNIEXPORT void JNICALL Java_org_cef_CefContext_N_1DoMessageLoopWork
  (JNIEnv *env, jclass) {
  CefDoMessageLoopWork();
}

JNIEXPORT jobject JNICALL Java_org_cef_CefContext_N_1CreateBrowser
  (JNIEnv *env, jclass, jobject handler, jlong windowHandle,
   jstring url, jboolean transparent, jobject canvas) {
#if defined(OS_MACOSX)
  if (!g_use_osr)
    return util_mac::CefCreateBrowserOnMainThread(handler, windowHandle, url, transparent, canvas);
#endif
  jobject browser = NewJNIObject(env, "org/cef/CefBrowser_N");
  if (!browser)
    return NULL;

  CefRefPtr<CefBrowser> browserObj;

  CefWindowInfo windowInfo;

  windowInfo.SetAsOffScreen((CefWindowHandle)windowHandle);
  windowInfo.SetTransparentPainting(transparent);

  CefRefPtr<ClientHandler> client = new ClientHandler(env, browser, handler);
  CefBrowserSettings settings;

  browserObj = CefBrowserHost::CreateBrowserSync(windowInfo, client.get(),
                                                 GetJNIString(env, url),
                                                 settings, NULL);
  SetCefForJNIObject(env, browser, browserObj.get());

  return browser;
}

JNIEXPORT jlong JNICALL Java_org_cef_CefContext_N_1GetWindowHandle
  (JNIEnv *, jclass, jlong displayHandle) {
  CefWindowHandle windowHandle = NULL;
#if defined(OS_WIN)
  windowHandle = ::WindowFromDC((HDC)displayHandle);
#elif defined(OS_LINUX)
  // TODO(jcef): The |displayHandle| argument is an X11 Window. We can't use it
  // until CEF has moved from GTK to Aura.
#elif defined(OS_MACOSX)
  ASSERT(util_mac::IsNSView((void*)displayHandle));
#endif
  return (jlong)windowHandle;
}
