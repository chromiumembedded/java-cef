// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "client_app.h"
#include "jni_util.h"
#include "util.h"
#if defined(OS_MACOSX)
#include "util_mac.h"
#endif

namespace {

std::set<std::string>& GetTempFilesSet() {
  static std::set<std::string> tempFiles;
  return tempFiles;
}

}  // namespace

ClientApp::ClientApp(const std::string& module_dir,
                     const std::string& cache_path,
                     const jobject app_handler)
    : module_dir_(module_dir), cache_path_(cache_path), app_handler_(NULL) {
  JNIEnv* env = GetJNIEnv();
  if (env)
    app_handler_ = env->NewGlobalRef(app_handler);
  process_handler_ = new BrowserProcessHandler(app_handler_);
}

ClientApp::~ClientApp() {
  if (!app_handler_)
    return;
  BEGIN_ENV(env)
  env->DeleteGlobalRef(app_handler_);
  END_ENV(env)
}

void ClientApp::OnBeforeCommandLineProcessing(
    const CefString& process_type,
    CefRefPtr<CefCommandLine> command_line) {
  // If the java code has registered an AppHandler, we'll forward
  // the commandline processing to it before we append the essential
  // switches "locale_pak" and "use-core-animation".
  if (app_handler_ != NULL && process_type.empty()) {
    BEGIN_ENV(env)
    jstring jprocess_type = NewJNIString(env, process_type);
    jobject jcommand_line =
        NewJNIObject(env, "org/cef/callback/CefCommandLine_N");
    if (jcommand_line != NULL) {
      SetCefForJNIObject(env, jcommand_line, command_line.get(),
                         "CefCommandLine");
      JNI_CALL_VOID_METHOD(
          env, app_handler_, "onBeforeCommandLineProcessing",
          "(Ljava/lang/String;Lorg/cef/callback/CefCommandLine;)V",
          jprocess_type, jcommand_line);
      SetCefForJNIObject<CefCommandLine>(env, jcommand_line, NULL,
                                         "CefCommandLine");
    }
    env->DeleteLocalRef(jcommand_line);
    env->DeleteLocalRef(jprocess_type);
    END_ENV(env)
  }

  if (process_type.empty()) {
#if defined(OS_MACOSX)
    // Specify a path for the locale.pak file because CEF will fail to locate
    // it based on the app bundle structure.
    const std::string& locale_path = util_mac::GetAbsPath(
        module_dir_ +
        "/../Frameworks/Chromium Embedded Framework.framework/"
        "Resources/en.lproj/locale.pak");
    command_line->AppendSwitchWithValue("locale_pak", locale_path);
    // If windowed rendering is used, we need the browser window as CALayer
    // due Java7 is CALayer based instead of NSLayer based.
    command_line->AppendSwitch("use-core-animation");
#endif  // defined(OS_MACOSX)

#if defined(OS_WIN)
    // To avoid shutdown issues extensions should not be used in combination
    // with multi-threaded-message-loop.
    command_line->AppendSwitch("disable-extensions");
#endif

    if (cache_path_.empty() &&
        !command_line->HasSwitch("disable-gpu-shader-disk-cache")) {
      // Don't create a "GPUCache" directory when cache_path is unspecified.
      command_line->AppendSwitch("disable-gpu-shader-disk-cache");
    }
  }
}

void ClientApp::OnRegisterCustomSchemes(
    CefRawPtr<CefSchemeRegistrar> registrar) {
  if (!app_handler_)
    return;

  BEGIN_ENV(env)
  jobject jregistrar =
      NewJNIObject(env, "org/cef/callback/CefSchemeRegistrar_N");
  if (jregistrar != NULL) {
    SetCefForJNIObject(env, jregistrar, registrar, "CefSchemeRegistrar");
    JNI_CALL_VOID_METHOD(env, app_handler_, "onRegisterCustomSchemes",
                         "(Lorg/cef/callback/CefSchemeRegistrar;)V",
                         jregistrar);
    SetCefForJNIObject<CefSchemeRegistrar>(env, jregistrar, NULL,
                                           "CefSchemeRegistrar");
  }
  env->DeleteLocalRef(jregistrar);
  END_ENV(env)
}

CefRefPtr<CefBrowserProcessHandler> ClientApp::GetBrowserProcessHandler() {
  return process_handler_.get();
}

#if defined(OS_MACOSX)
bool ClientApp::HandleTerminate() {
  BEGIN_ENV(env)
  jclass cls = FindClass(env, "org/cef/CefApp");
  if (!cls) {
    return false;
  }

  jmethodID methodId =
      env->GetStaticMethodID(cls, "getInstance", "()Lorg/cef/CefApp;");
  if (!methodId) {
    return false;
  }

  jobject jcefApp = env->CallStaticObjectMethod(cls, methodId);
  if (!jcefApp) {
    return false;
  }

  JNI_CALL_VOID_METHOD(env, jcefApp, "handleBeforeTerminate", "()V");
  END_ENV(env)
  return true;
}
#endif  // defined(OS_MACOSX)

// static
void ClientApp::registerTempFile(const std::string& tmpFile) {
  GetTempFilesSet().insert(tmpFile);
}

// static
void ClientApp::eraseTempFiles() {
  std::set<std::string>& tempFiles = GetTempFilesSet();
  std::set<std::string>::iterator iter;
  for (iter = tempFiles.begin(); iter != tempFiles.end(); ++iter) {
    remove((*iter).c_str());
  }
  tempFiles.clear();
}
