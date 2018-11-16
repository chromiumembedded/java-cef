// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "CefApp.h"

#include <string>

#include "include/cef_app.h"
#include "include/cef_version.h"

#include "context.h"
#include "jcef_version.h"
#include "jni_util.h"
#include "scheme_handler_factory.h"
#include "util.h"

#if defined(OS_LINUX)
#include <X11/Xlib.h>
#endif

JNIEXPORT jboolean JNICALL Java_org_cef_CefApp_N_1PreInitialize(JNIEnv* env,
                                                                jobject c) {
#if !defined(OS_MACOSX)
  // On macOS this is called from Startup().
  Context::Create();
#endif
  return Context::GetInstance()->PreInitialize(env, c) ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jboolean JNICALL
Java_org_cef_CefApp_N_1Initialize(JNIEnv* env,
                                  jobject c,
                                  jstring argPathToJavaDLL,
                                  jobject appHandler,
                                  jobject jsettings) {
  return Context::GetInstance()->Initialize(env, c, argPathToJavaDLL,
                                            appHandler, jsettings)
             ? JNI_TRUE
             : JNI_FALSE;
}

JNIEXPORT void JNICALL Java_org_cef_CefApp_N_1Shutdown(JNIEnv* env, jobject) {
  Context::GetInstance()->Shutdown();
  Context::Destroy();
}

JNIEXPORT void JNICALL Java_org_cef_CefApp_N_1DoMessageLoopWork(JNIEnv* env,
                                                                jobject) {
  Context::GetInstance()->DoMessageLoopWork();
}

JNIEXPORT jobject JNICALL Java_org_cef_CefApp_N_1GetVersion(JNIEnv* env,
                                                            jobject obj) {
  return NewJNIObject(env, "org/cef/CefApp$CefVersion",
                      "(Lorg/cef/CefApp;IIIIIII)V", obj, JCEF_COMMIT_NUMBER,
                      cef_version_info(0),   // CEF_VERSION_MAJOR
                      cef_version_info(1),   // CEF_COMMIT_NUMBER
                      cef_version_info(2),   // CHROME_VERSION_MAJOR
                      cef_version_info(3),   // CHROME_VERSION_MINOR
                      cef_version_info(4),   // CHROME_VERSION_BUILD
                      cef_version_info(5));  // CHROME_VERSION_PATCH
}

JNIEXPORT jboolean JNICALL
Java_org_cef_CefApp_N_1RegisterSchemeHandlerFactory(JNIEnv* env,
                                                    jobject,
                                                    jstring jSchemeName,
                                                    jstring jDomainName,
                                                    jobject jFactory) {
  if (!jFactory)
    return JNI_FALSE;

  CefRefPtr<SchemeHandlerFactory> factory =
      new SchemeHandlerFactory(env, jFactory);
  if (!factory)
    return JNI_FALSE;

  bool result = CefRegisterSchemeHandlerFactory(GetJNIString(env, jSchemeName),
                                                GetJNIString(env, jDomainName),
                                                factory.get());
  return result ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jboolean JNICALL
Java_org_cef_CefApp_N_1ClearSchemeHandlerFactories(JNIEnv*, jobject) {
  return CefClearSchemeHandlerFactories() ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jboolean JNICALL Java_org_cef_CefApp_N_1Startup(JNIEnv*, jclass) {
#if defined(OS_LINUX)
  XInitThreads();
#elif defined(OS_MACOSX)
  // Load the CEF framework library at runtime instead of linking directly
  // as required by the macOS sandbox implementation.
  scoped_ptr<CefScopedLibraryLoader> library_loader(new CefScopedLibraryLoader);
  if (!library_loader->LoadInMain())
    return JNI_FALSE;

  // The Context object has members that can't be initialized until after the
  // CEF framework is loaded.
  Context::Create();
  Context::GetInstance()->set_library_loader(library_loader.release());
#endif  // defined(OS_MACOSX)
  return JNI_TRUE;
}
