// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "CefRequestContext_N.h"
#include "include/cef_request_context.h"
#include "request_context_handler.h"
#include "jni_util.h"

JNIEXPORT jobject JNICALL Java_org_cef_browser_CefRequestContext_1N_N_1GetGlobalContext
  (JNIEnv *env, jclass cls) {
  CefRefPtr<CefRequestContext> context = CefRequestContext::GetGlobalContext();
  if (!context.get())
    return NULL;

  jobject jContext = NewJNIObject(env, cls);
  if (!jContext)
    return NULL;

  SetCefForJNIObject(env, jContext, context.get(), "CefRequestContext");
  return jContext;
}

JNIEXPORT jobject JNICALL Java_org_cef_browser_CefRequestContext_1N_N_1CreateContext
  (JNIEnv *env, jclass cls, jobject jhandler) {
  CefRefPtr<CefRequestContextHandler> handler = NULL;
  if (jhandler != NULL) {
    handler = new RequestContextHandler(env, jhandler);
  }

  // TODO(JCEF): Expose CefRequestContextSettings.
  CefRequestContextSettings settings;
  CefRefPtr<CefRequestContext> context = CefRequestContext::CreateContext(settings, handler);
  if (!context.get())
    return NULL;

  jobject jContext = NewJNIObject(env, cls);
  if (!jContext)
    return NULL;

  SetCefForJNIObject(env, jContext, context.get(), "CefRequestContext");
  return jContext;
}

JNIEXPORT jboolean JNICALL Java_org_cef_browser_CefRequestContext_1N_N_1IsGlobal
  (JNIEnv *env, jobject obj) {
  CefRefPtr<CefRequestContext> context =
      GetCefFromJNIObject<CefRequestContext>(env, obj, "CefRequestContext");
  if (!context.get())
    return JNI_FALSE;
  return context->IsGlobal() ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT void JNICALL Java_org_cef_browser_CefRequestContext_1N_N_1CefRequestContext_1DTOR
  (JNIEnv *env, jobject obj) {
  SetCefForJNIObject<CefRequestContext>(env, obj, NULL, "CefRequestContext");
}
