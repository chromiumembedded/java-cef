// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "CefApp.h"

#include <string>

#include "include/cef_app.h"
#include "include/cef_browser.h"
#include "include/cef_path_util.h"
#include "include/cef_version.h"
#include "browser_process_handler.h"
#include "client_app.h"
#include "client_handler.h"
#include "jcef_version.h"
#include "jni_util.h"
#include "render_handler.h"
#include "scheme_handler_factory.h"
#include "util.h"

#if defined(OS_LINUX)
#include <gdk/gdkx.h>
#endif

#if defined(OS_MACOSX)
#include "util_mac.h"
#endif

#if defined(OS_POSIX)
#include "signal_restore_posix.h"
#endif

JNIEXPORT jboolean JNICALL Java_org_cef_CefApp_N_1Initialize
  (JNIEnv *env, jobject c, jstring argPathToJavaDLL, jobject appHandler,
    jobject jsettings) {
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

  CefSettings settings = GetJNISettings(env, jsettings);  

  // Sandbox is not supported because:
  // - Use of a separate sub-process executable on Windows.
  // - Use of a temporary file to communicate custom schemes to the
  //   renderer process.
  settings.no_sandbox = true;

#if defined(OS_WIN)
  // Required to fix issues with drag&drop on Windows.
  settings.multi_threaded_message_loop = true;
#endif

  CefRefPtr<ClientApp> client_app(new ClientApp(module_dir, appHandler));
  bool res = false;

#if defined(OS_POSIX)
  // CefInitialize will reset signal handlers. Backup/restore the original
  // signal handlers to avoid crashes in the JVM (see issue #41).
  BackupSignalHandlers();
#endif

#if defined(OS_MACOSX)
  res = util_mac::CefInitializeOnMainThread(main_args, settings,
                                             client_app.get());
#else
  res = CefInitialize(main_args, settings, client_app.get(), NULL);
#endif

#if defined(OS_POSIX)
  RestoreSignalHandlers();
#endif

  return res ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT void JNICALL Java_org_cef_CefApp_N_1Shutdown
  (JNIEnv *env, jobject) {
  ClientApp::eraseTempFiles();
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

JNIEXPORT jobject JNICALL Java_org_cef_CefApp_N_1GetVersion
  (JNIEnv *env, jobject obj) {
  return NewJNIObject(env,
                      "org/cef/CefApp$CefVersion",
                      "(Lorg/cef/CefApp;IIIIIII)V",
                      obj,
                      JCEF_REVISION,
                      cef_version_info(0),  // CEF_VERSION_MAJOR
                      cef_version_info(1),  // CEF_REVISION
                      cef_version_info(2),  // CHROME_VERSION_MAJOR
                      cef_version_info(3),  // CHROME_VERSION_MINOR
                      cef_version_info(4),  // CHROME_VERSION_BUILD
                      cef_version_info(5)); // CHROME_VERSION_PATCH
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

JNIEXPORT void JNICALL Java_org_cef_CefApp_N_1ContinueDefaultTerminate
  (JNIEnv *, jobject) {
#if defined(OS_MACOSX)
  util_mac::ContinueDefaultTerminate();
#endif
}
