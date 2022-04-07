// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "request_context_handler.h"

#include "jni_util.h"
#include "resource_request_handler.h"
#include "util.h"

RequestContextHandler::RequestContextHandler(JNIEnv* env, jobject jhandler)
    : handle_(env, jhandler) {}

CefRefPtr<CefResourceRequestHandler>
RequestContextHandler::GetResourceRequestHandler(
    CefRefPtr<CefBrowser> browser,
    CefRefPtr<CefFrame> frame,
    CefRefPtr<CefRequest> request,
    bool is_navigation,
    bool is_download,
    const CefString& request_initiator,
    bool& disable_default_handling) {
  ScopedJNIEnv env;
  if (!env)
    return nullptr;

  ScopedJNIBrowser jbrowser(env, browser);
  ScopedJNIFrame jframe(env, frame);
  jframe.SetTemporary();
  ScopedJNIRequest jrequest(env, request);
  jrequest.SetTemporary();
  ScopedJNIString jrequestInitiator(env, request_initiator);
  ScopedJNIBoolRef jdisableDefaultHandling(env, disable_default_handling);
  ScopedJNIObjectResult jresult(env);

  JNI_CALL_METHOD(env, handle_, "getResourceRequestHandler",
                  "(Lorg/cef/browser/CefBrowser;Lorg/cef/browser/CefFrame;Lorg/"
                  "cef/network/CefRequest;ZZLjava/lang/String;Lorg/cef/misc/"
                  "BoolRef;)Lorg/cef/handler/CefResourceRequestHandler;",
                  Object, jresult, jbrowser.get(), jframe.get(), jrequest.get(),
                  is_navigation ? JNI_TRUE : JNI_FALSE,
                  is_download ? JNI_TRUE : JNI_FALSE, jrequestInitiator.get(),
                  jdisableDefaultHandling.get());

  disable_default_handling = jdisableDefaultHandling;

  if (jresult)
    return new ResourceRequestHandler(env, jresult);
  return nullptr;
}
