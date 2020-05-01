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

ClientApp::ClientApp(const std::string& cache_path,
                     JNIEnv* env,
                     const jobject app_handler)
    : cache_path_(cache_path),
      handle_(env, app_handler),
      process_handler_(new BrowserProcessHandler(env, app_handler)) {}

void ClientApp::OnBeforeCommandLineProcessing(
    const CefString& process_type,
    CefRefPtr<CefCommandLine> command_line) {
  // If the java code has registered an AppHandler, we'll forward
  // the commandline processing to it before we append the essential
  // switches "locale_pak" and "use-core-animation".
  if (handle_ && process_type.empty()) {
    ScopedJNIEnv env;
    if (!env)
      return;

    ScopedJNIString jprocessType(env, process_type);
    ScopedJNIObject<CefCommandLine> jcommandLine(
        env, command_line, "org/cef/callback/CefCommandLine_N",
        "CefCommandLine");
    jcommandLine.SetTemporary();

    JNI_CALL_VOID_METHOD(
        env, handle_, "onBeforeCommandLineProcessing",
        "(Ljava/lang/String;Lorg/cef/callback/CefCommandLine;)V",
        jprocessType.get(), jcommandLine.get());
  }

  if (process_type.empty()) {
#if defined(OS_MACOSX)
    // If windowed rendering is used, we need the browser window as CALayer
    // due Java7 is CALayer based instead of NSLayer based.
    command_line->AppendSwitch("use-core-animation");

    // Skip keychain prompt on startup.
    command_line->AppendSwitch("use-mock-keychain");
#endif  // defined(OS_MACOSX)

    if (cache_path_.empty() &&
        !command_line->HasSwitch("disable-gpu-shader-disk-cache")) {
      // Don't create a "GPUCache" directory when cache_path is unspecified.
      command_line->AppendSwitch("disable-gpu-shader-disk-cache");
    }
  }
}

void ClientApp::OnRegisterCustomSchemes(
    CefRawPtr<CefSchemeRegistrar> registrar) {
  if (!handle_)
    return;

  ScopedJNIEnv env;
  if (!env)
    return;

  ScopedJNIObject<CefSchemeRegistrar, CefRawPtr<CefSchemeRegistrar>> jregistrar(
      env, registrar, "org/cef/callback/CefSchemeRegistrar_N",
      "CefSchemeRegistrar");
  jregistrar.SetTemporary();

  JNI_CALL_VOID_METHOD(env, handle_, "onRegisterCustomSchemes",
                       "(Lorg/cef/callback/CefSchemeRegistrar;)V",
                       jregistrar.get());
}

CefRefPtr<CefBrowserProcessHandler> ClientApp::GetBrowserProcessHandler() {
  return process_handler_.get();
}

#if defined(OS_MACOSX)
bool ClientApp::HandleTerminate() {
  ScopedJNIEnv env;
  if (!env)
    return false;

  ScopedJNIClass jcls(env, "org/cef/CefApp");
  if (!jcls) {
    return false;
  }

  jmethodID methodId =
      env->GetStaticMethodID(jcls, "getInstance", "()Lorg/cef/CefApp;");
  if (!methodId) {
    return false;
  }

  ScopedJNIObjectLocal jcefApp(env,
                               env->CallStaticObjectMethod(jcls, methodId));
  if (!jcefApp) {
    return false;
  }

  JNI_CALL_VOID_METHOD(env, jcefApp, "handleBeforeTerminate", "()V");
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
