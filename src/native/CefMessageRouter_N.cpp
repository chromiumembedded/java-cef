// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "CefMessageRouter_N.h"

#include "include/base/cef_bind.h"
#include "include/wrapper/cef_closure_task.h"
#include "include/wrapper/cef_message_router.h"

#include "message_router_handler.h"
#include "jni_util.h"

JNIEXPORT void JNICALL Java_org_cef_browser_CefMessageRouter_1N_N_1Create
  (JNIEnv *env, jobject obj, jobject jrouterConfig) {
  CefMessageRouterConfig config = GetJNIMessageRouterConfig(env, jrouterConfig);

  CefRefPtr<CefMessageRouterBrowserSide> msgRouter =
      CefMessageRouterBrowserSide::Create(config);
  if (!msgRouter.get())
    return;
  SetCefForJNIObject(env, obj, msgRouter.get(), "CefMessageRouter");
}

JNIEXPORT void JNICALL Java_org_cef_browser_CefMessageRouter_1N_N_1Dispose
  (JNIEnv *env, jobject obj) {
  SetCefForJNIObject<CefMessageRouterBrowserSide>(env, obj, NULL, "CefMessageRouter");
}

JNIEXPORT jboolean JNICALL Java_org_cef_browser_CefMessageRouter_1N_N_1AddHandler
  (JNIEnv *env, jobject obj, jobject jrouterHandler, jboolean jfirst) {
  CefRefPtr<CefMessageRouterBrowserSide> msgRouter =
      GetCefFromJNIObject<CefMessageRouterBrowserSide>(env, obj, "CefMessageRouter");
  if (!msgRouter.get())
    return JNI_FALSE;

  // get native reference or create a new one
  CefRefPtr<MessageRouterHandler> routerHandler =
      GetCefFromJNIObject<MessageRouterHandler>(env, jrouterHandler, "CefMessageRouterHandler");
  if (!routerHandler.get()) {
    routerHandler = new MessageRouterHandler(env, jrouterHandler);
    SetCefForJNIObject(env, jrouterHandler, routerHandler.get(), "CefMessageRouterHandler");
  }

  bool result = false;
  if (CefCurrentlyOn(TID_UI)) {
    result = msgRouter->AddHandler(routerHandler, (jfirst != JNI_FALSE));
  } else {
    result = CefPostTask(TID_UI,
        base::Bind(base::IgnoreResult(&CefMessageRouterBrowserSide::AddHandler),
                   msgRouter.get(),
                   routerHandler,
                   (jfirst != JNI_FALSE)));
  }
  return result ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jboolean JNICALL Java_org_cef_browser_CefMessageRouter_1N_N_1RemoveHandler
  (JNIEnv *env, jobject obj, jobject jrouterHandler) {
  CefRefPtr<CefMessageRouterBrowserSide> msgRouter =
      GetCefFromJNIObject<CefMessageRouterBrowserSide>(env, obj, "CefMessageRouter");
  if (!msgRouter.get())
    return JNI_FALSE;

  CefRefPtr<MessageRouterHandler> routerHandler =
      GetCefFromJNIObject<MessageRouterHandler>(env, jrouterHandler, "CefMessageRouterHandler");
  if (!routerHandler.get())
    return JNI_FALSE;

  bool result = false;
  if (CefCurrentlyOn(TID_UI)) {
    result = msgRouter->RemoveHandler(routerHandler);
  } else {
    result = CefPostTask(TID_UI,
        base::Bind(base::IgnoreResult(&CefMessageRouterBrowserSide::RemoveHandler),
                   msgRouter.get(),
                   routerHandler));
  }
  return result ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT void JNICALL Java_org_cef_browser_CefMessageRouter_1N_N_1CancelPending
  (JNIEnv *env, jobject obj, jobject jbrowser, jobject jrouterHandler) {
  CefRefPtr<CefMessageRouterBrowserSide> msgRouter =
      GetCefFromJNIObject<CefMessageRouterBrowserSide>(env, obj, "CefMessageRouter");
  if (!msgRouter.get())
    return;

  // browser and/or routerHandler may be null
  CefRefPtr<CefBrowser> browser =
      GetCefFromJNIObject<CefBrowser>(env, jbrowser, "CefBrowser");
  CefRefPtr<MessageRouterHandler> routerHandler =
      GetCefFromJNIObject<MessageRouterHandler>(env, jrouterHandler, "CefMessageRouterHandler");

  msgRouter->CancelPending(browser, routerHandler);
}

JNIEXPORT jint JNICALL Java_org_cef_browser_CefMessageRouter_1N_N_1GetPendingCount
  (JNIEnv *env, jobject obj, jobject jbrowser, jobject jrouterHandler) {
  CefRefPtr<CefMessageRouterBrowserSide> msgRouter =
      GetCefFromJNIObject<CefMessageRouterBrowserSide>(env, obj, "CefMessageRouter");
  if (!msgRouter.get())
    return 0;

  // browser and/or routerHandler may be null
  CefRefPtr<CefBrowser> browser =
      GetCefFromJNIObject<CefBrowser>(env, jbrowser, "CefBrowser");
  CefRefPtr<MessageRouterHandler> routerHandler =
      GetCefFromJNIObject<MessageRouterHandler>(env, jrouterHandler, "CefMessageRouterHandler");

  return msgRouter->GetPendingCount(browser, routerHandler);
}
