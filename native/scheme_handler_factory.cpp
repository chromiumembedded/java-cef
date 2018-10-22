// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "scheme_handler_factory.h"

#include "jni_util.h"
#include "resource_handler.h"

SchemeHandlerFactory::SchemeHandlerFactory(JNIEnv* env, jobject jfactory) {
  jfactory_ = env->NewGlobalRef(jfactory);
}

SchemeHandlerFactory::~SchemeHandlerFactory() {
  JNIEnv* env = GetJNIEnv();
  env->DeleteGlobalRef(jfactory_);
}

CefRefPtr<CefResourceHandler> SchemeHandlerFactory::Create(
    CefRefPtr<CefBrowser> browser,
    CefRefPtr<CefFrame> frame,
    const CefString& scheme_name,
    CefRefPtr<CefRequest> request) {
  JNIEnv* env = GetJNIEnv();
  if (!env)
    return NULL;

  jobject jRequest = NewJNIObject(env, "org/cef/network/CefRequest_N");
  if (jRequest != NULL)
    SetCefForJNIObject(env, jRequest, request.get(), "CefRequest");

  jobject jResourceHandler = NULL;
  jobject jframe = GetJNIFrame(env, frame);
  jstring jscheme_name = NewJNIString(env, scheme_name);
  JNI_CALL_METHOD(env, jfactory_, "create",
                  "(Lorg/cef/browser/CefBrowser;Lorg/cef/browser/"
                  "CefFrame;Ljava/lang/String;Lorg/cef/"
                  "network/CefRequest;)Lorg/cef/handler/CefResourceHandler;",
                  Object, jResourceHandler, GetJNIBrowser(browser), jframe,
                  jscheme_name, jRequest);

  if (jRequest != NULL)
    SetCefForJNIObject<CefRequest>(env, jRequest, NULL, "CefRequest");

  env->DeleteLocalRef(jframe);
  env->DeleteLocalRef(jscheme_name);
  env->DeleteLocalRef(jRequest);

  if (!jResourceHandler)
    return NULL;
  CefRefPtr<CefResourceHandler> result =
      new ResourceHandler(env, jResourceHandler);
  return result;
}
