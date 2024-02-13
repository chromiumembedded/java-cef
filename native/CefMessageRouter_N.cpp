// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "CefMessageRouter_N.h"

#include "include/base/cef_callback.h"
#include "include/wrapper/cef_closure_task.h"
#include "include/wrapper/cef_message_router.h"

#include "jni_scoped_helpers.h"
#include "jni_util.h"
#include "message_router_handler.h"

namespace {

const char kCefClassName[] = "CefMessageRouter";

CefRefPtr<CefMessageRouter> GetSelf(jlong self) {
  return reinterpret_cast<CefMessageRouter*>(self);
}

CefRefPtr<MessageRouterHandler> GetHandler(JNIEnv* env,
                                           jobject jrouterHandler,
                                           bool allow_create) {
  ScopedJNIObject<MessageRouterHandler> jrouterHandlerObj(
      env, jrouterHandler, false /* should_delete */,
      "CefMessageRouterHandler");
  return allow_create ? jrouterHandlerObj.GetOrCreateCefObject()
                      : jrouterHandlerObj.GetCefObject();
}

}  // namespace

JNIEXPORT jobject JNICALL
Java_org_cef_browser_CefMessageRouter_1N_N_1Create(JNIEnv* env,
                                                   jclass cls,
                                                   jobject jrouterConfig) {
  CefMessageRouterConfig config = GetJNIMessageRouterConfig(env, jrouterConfig);
  CefRefPtr<CefMessageRouterBrowserSide> msgRouter =
      CefMessageRouterBrowserSide::Create(config);
  ScopedJNIMessageRouter jmsgRouter(env, msgRouter);

  JNI_CALL_VOID_METHOD(
      env, jmsgRouter, "setMessageRouterConfig",
      "(Lorg/cef/browser/CefMessageRouter$CefMessageRouterConfig;)V",
      jrouterConfig);

  return jmsgRouter.Release();
}

JNIEXPORT void JNICALL
Java_org_cef_browser_CefMessageRouter_1N_N_1Dispose(JNIEnv* env,
                                                    jobject obj,
                                                    jlong self) {
  SetCefForJNIObject<CefMessageRouterBrowserSide>(env, obj, nullptr,
                                                  kCefClassName);
}

JNIEXPORT jboolean JNICALL
Java_org_cef_browser_CefMessageRouter_1N_N_1AddHandler(JNIEnv* env,
                                                       jobject obj,
                                                       jlong self,
                                                       jobject jrouterHandler,
                                                       jboolean jfirst) {
  CefRefPtr<CefMessageRouter> msgRouter = GetSelf(self);
  if (!msgRouter)
    return JNI_FALSE;

  CefRefPtr<MessageRouterHandler> routerHandler =
      GetHandler(env, jrouterHandler, true /* allow_create */);

  if (CefCurrentlyOn(TID_UI)) {
    msgRouter->AddHandler(routerHandler.get(), (jfirst != JNI_FALSE));
  } else {
    CefPostTask(TID_UI, base::BindOnce(
                            [](CefRefPtr<CefMessageRouter> msgRouter,
                               CefRefPtr<MessageRouterHandler> routerHandler,
                               bool first) {
                              msgRouter->AddHandler(routerHandler.get(), first);
                            },
                            msgRouter, routerHandler, (jfirst != JNI_FALSE)));
  }
  return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_org_cef_browser_CefMessageRouter_1N_N_1RemoveHandler(
    JNIEnv* env,
    jobject obj,
    jlong self,
    jobject jrouterHandler) {
  CefRefPtr<CefMessageRouter> msgRouter = GetSelf(self);
  if (!msgRouter)
    return JNI_FALSE;

  CefRefPtr<MessageRouterHandler> routerHandler =
      GetHandler(env, jrouterHandler, false /* allow_create */);
  if (!routerHandler)
    return JNI_FALSE;

  if (CefCurrentlyOn(TID_UI)) {
    msgRouter->RemoveHandler(routerHandler.get());
  } else {
    CefPostTask(TID_UI, base::BindOnce(
                            [](CefRefPtr<CefMessageRouter> msgRouter,
                               CefRefPtr<MessageRouterHandler> routerHandler) {
                              msgRouter->RemoveHandler(routerHandler.get());
                            },
                            msgRouter, routerHandler));
  }

  // Remove JNI reference on jrouterhandler added by the ScopedJNIObject
  SetCefForJNIObject<MessageRouterHandler>(env, jrouterHandler, nullptr,
                                           "CefMessageRouterHandler");

  return JNI_TRUE;
}

JNIEXPORT void JNICALL
Java_org_cef_browser_CefMessageRouter_1N_N_1CancelPending(
    JNIEnv* env,
    jobject obj,
    jlong self,
    jobject jbrowser,
    jobject jrouterHandler) {
  CefRefPtr<CefMessageRouter> msgRouter = GetSelf(self);
  if (!msgRouter)
    return;

  // Browser and/or routerHandler may be null.
  CefRefPtr<CefBrowser> browser = GetJNIBrowser(env, jbrowser);
  CefRefPtr<MessageRouterHandler> routerHandler =
      GetHandler(env, jrouterHandler, false /* allow_create */);

  msgRouter->CancelPending(browser, routerHandler.get());
}
