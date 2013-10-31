// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "CefContext.h"
#include "include/cef_app.h"
#include "include/cef_browser.h"
#include "include/cef_path_util.h"
#include "client_handler.h"
#include "jni_util.h"
#include "util.h"

JNIEXPORT jboolean JNICALL Java_org_cef_CefContext_N_1Initialize
  (JNIEnv *env, jclass c, jstring argPathToJavaDLL, jstring cachePath) {
  JavaVM* jvm;
  jint rs = env->GetJavaVM(&jvm);
  ASSERT(rs == JNI_OK);
  SetJVM(jvm);

  CefMainArgs main_args(::GetModuleHandle(NULL));
	CefSettings settings;

  CefString module_dir = GetJNIString(env, argPathToJavaDLL);
  
#if defined(OS_WIN)
  module_dir = module_dir.ToString() + "\\jcef_helper.exe";
#else
  module_dir = module_dir.ToString() + "/jcef_helper";
#endif
  CefString(&settings.browser_subprocess_path) = module_dir;

	if(!CefInitialize(main_args, settings, NULL))
		return JNI_FALSE;

	return JNI_TRUE;
}

JNIEXPORT void JNICALL Java_org_cef_CefContext_N_1Shutdown
  (JNIEnv *env, jclass) {
	CefShutdown();
}

JNIEXPORT void JNICALL Java_org_cef_CefContext_N_1DoMessageLoopWork
  (JNIEnv *env, jclass) {
  CefDoMessageLoopWork();
}

JNIEXPORT jobject JNICALL Java_org_cef_CefContext_N_1CreateBrowser
  (JNIEnv *env, jclass, jobject handler, jlong windowHandle,
   jstring url, jboolean transparent) {
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
#endif
  return (jlong)windowHandle;
}
