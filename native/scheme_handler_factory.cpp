// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "scheme_handler_factory.h"

#include "jni_util.h"
#include "resource_handler.h"

SchemeHandlerFactory::SchemeHandlerFactory(JNIEnv* env, jobject jfactory)
    : handle_(env, jfactory) {}

CefRefPtr<CefResourceHandler> SchemeHandlerFactory::Create(
    CefRefPtr<CefBrowser> browser,
    CefRefPtr<CefFrame> frame,
    const CefString& scheme_name,
    CefRefPtr<CefRequest> request) {
  ScopedJNIEnv env;
  if (!env)
    return nullptr;

  ScopedJNIBrowser jbrowser(env, browser);
  ScopedJNIFrame jframe(env, frame);
  jframe.SetTemporary();
  ScopedJNIString jschemeName(env, scheme_name);
  ScopedJNIRequest jrequest(env, request);
  jrequest.SetTemporary();
  ScopedJNIObjectResult jresult(env);

  JNI_CALL_METHOD(env, handle_, "create",
                  "(Lorg/cef/browser/CefBrowser;Lorg/cef/browser/"
                  "CefFrame;Ljava/lang/String;Lorg/cef/"
                  "network/CefRequest;)Lorg/cef/handler/CefResourceHandler;",
                  Object, jresult, jbrowser.get(), jframe.get(),
                  jschemeName.get(), jrequest.get());

  if (jresult)
    return new ResourceHandler(env, jresult);
  return nullptr;
}
