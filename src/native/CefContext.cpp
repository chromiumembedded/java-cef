// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "CefContext.h"

#include <string>

#include "include/cef_app.h"
#include "include/cef_browser.h"
#include "include/cef_path_util.h"
#include "client_handler.h"
#include "render_handler.h"
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
  explicit ClientApp(const std::string& module_dir,
                     const std::vector<CefString> args)
      : module_dir_(module_dir), args_(args) {
  }

  virtual void OnBeforeCommandLineProcessing(
      const CefString& process_type,
      CefRefPtr<CefCommandLine> command_line) OVERRIDE {
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
      if (!g_use_osr)
        command_line->AppendSwitch("use-core-animation");
#endif  // defined(OS_MACOSX)

      // Forward switches and arguments from Java to Cef
      bool parseSwitchesDone = false;
      for (size_t i=0;i< args_.size();i++){
        CefString tmp = args_.at(i);
        if (parseSwitchesDone || tmp.length() < 2) {
          command_line->AppendArgument(tmp);
          continue;
        }
        // Arguments with '--', '-' and, on Windows, '/' prefixes are considered
        // switches.
        std::string cppStr = tmp.ToString();
        size_t switchCnt = cppStr.find("--") == 0 ? 2 :
#if defined(OS_WIN)
                           cppStr.find("/") == 0 ? 1 :
#endif
                           cppStr.find("-") == 0 ? 1 : 0;
        switch (switchCnt) {
          case 2:
            // An argument of "--" will terminate switch parsing with all
            // subsequent tokens
            if (cppStr.length() == 2) {
              parseSwitchesDone = true;
              continue;
            }
            // FALL THRU
          case 1: {
            // Switches can optionally have a value specified using the '='
            // delimiter (e.g. "-switch=value").
            size_t equalPos = cppStr.find("=",switchCnt);
            if (equalPos != std::string::npos) {
              command_line->AppendSwitchWithValue(
                  cppStr.substr(switchCnt, equalPos - switchCnt),
                  cppStr.substr(equalPos + 1));
            } else {
              command_line->AppendSwitch(cppStr.substr(switchCnt));
            }
            break;
          }
          case 0:
            command_line->AppendArgument(tmp);
            break;
        }
      }
    }
  }

 private:
  std::string module_dir_;
  std::vector<CefString> args_;

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
   jboolean osrEnabled, jobjectArray args) {
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

  settings.no_sandbox = true;

#if defined(OS_LINUX)
  CefString(&settings.resources_dir_path) = module_dir;
  CefString(&settings.locales_dir_path) = module_dir + "/locales";
#endif

  std::vector<CefString> vals;
  GetJNIStringArray(env, args, vals);
  CefRefPtr<ClientApp> client_app(new ClientApp(module_dir, vals));
#if defined(OS_MACOSX)
  if (!g_use_osr) {
    return util_mac::CefInitializeOnMainThread(main_args, settings,
                                                client_app.get()) ?
        JNI_TRUE : JNI_FALSE;
  }
#endif
  return CefInitialize(main_args, settings, client_app.get(), NULL) ?
      JNI_TRUE : JNI_FALSE;
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
  if (!g_use_osr) {
    return util_mac::CefCreateBrowserOnMainThread(handler, windowHandle, url,
                                                   transparent, canvas);
  }
#endif
  jobject browser = NewJNIObject(env, "org/cef/CefBrowser_N");
  if (!browser)
    return NULL;

  CefRefPtr<CefBrowser> browserObj;

  CefRefPtr<ClientHandler> client = GetCefFromJNIObject<ClientHandler>(env, handler, "CefClientHandler");
  client->SetJBrowser(browser);

  CefWindowInfo windowInfo;
#if defined(OS_WIN)
  if (!g_use_osr) {
    HWND parent = GetHwndOfCanvas(canvas, env);
    CefRect rect;
    CefRefPtr<RenderHandler> renderHandler = (RenderHandler*)client->GetRenderHandler().get();
    if (renderHandler.get()) {
      renderHandler->GetViewRect(NULL, rect);
    }
    RECT winRect = {0,0, rect.width, rect.height};
    windowInfo.SetAsChild(parent,winRect);
  }
  else
#endif
  {
    windowInfo.SetAsOffScreen((CefWindowHandle)windowHandle);
    windowInfo.SetTransparentPainting(transparent);
  }
  CefBrowserSettings settings;

  browserObj = CefBrowserHost::CreateBrowserSync(windowInfo, client.get(),
                                                 GetJNIString(env, url),
                                                 settings, NULL);
  SetCefForJNIObject(env, browser, browserObj.get(), "CefBrowser");

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
