// Copyright (c) 2019 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "resource_request_handler.h"

#include "cookie_access_filter.h"
#include "jni_util.h"
#include "resource_handler.h"
#include "util.h"

ResourceRequestHandler::ResourceRequestHandler(JNIEnv* env, jobject handler)
    : handle_(env, handler) {}

CefRefPtr<CefCookieAccessFilter> ResourceRequestHandler::GetCookieAccessFilter(
    CefRefPtr<CefBrowser> browser,
    CefRefPtr<CefFrame> frame,
    CefRefPtr<CefRequest> request) {
  ScopedJNIEnv env;
  if (!env)
    return nullptr;

  ScopedJNIBrowser jbrowser(env, browser);
  ScopedJNIFrame jframe(env, frame);
  jframe.SetTemporary();
  ScopedJNIRequest jrequest(env, request);
  jrequest.SetTemporary();
  ScopedJNIObjectResult jresult(env);

  JNI_CALL_METHOD(env, handle_, "getCookieAccessFilter",
                  "(Lorg/cef/browser/CefBrowser;Lorg/cef/browser/CefFrame;Lorg/"
                  "cef/network/CefRequest;)"
                  "Lorg/cef/handler/CefCookieAccessFilter;",
                  Object, jresult, jbrowser.get(), jframe.get(),
                  jrequest.get());

  if (jresult)
    return new CookieAccessFilter(env, jresult);
  return nullptr;
}

// TODO(JCEF): Expose the |callback| parameter.
ResourceRequestHandler::ReturnValue
ResourceRequestHandler::OnBeforeResourceLoad(CefRefPtr<CefBrowser> browser,
                                             CefRefPtr<CefFrame> frame,
                                             CefRefPtr<CefRequest> request,
                                             CefRefPtr<CefCallback> callback) {
  ScopedJNIEnv env;
  if (!env)
    return RV_CONTINUE;

  ScopedJNIBrowser jbrowser(env, browser);
  ScopedJNIFrame jframe(env, frame);
  jframe.SetTemporary();
  ScopedJNIRequest jrequest(env, request);
  jrequest.SetTemporary();
  jboolean result = JNI_FALSE;

  JNI_CALL_METHOD(env, handle_, "onBeforeResourceLoad",
                  "(Lorg/cef/browser/CefBrowser;Lorg/cef/browser/CefFrame;Lorg/"
                  "cef/network/CefRequest;)Z",
                  Boolean, result, jbrowser.get(), jframe.get(),
                  jrequest.get());

  return (result != JNI_FALSE) ? RV_CANCEL : RV_CONTINUE;
}

CefRefPtr<CefResourceHandler> ResourceRequestHandler::GetResourceHandler(
    CefRefPtr<CefBrowser> browser,
    CefRefPtr<CefFrame> frame,
    CefRefPtr<CefRequest> request) {
  ScopedJNIEnv env;
  if (!env)
    return nullptr;

  ScopedJNIBrowser jbrowser(env, browser);
  ScopedJNIFrame jframe(env, frame);
  jframe.SetTemporary();
  ScopedJNIRequest jrequest(env, request);
  jrequest.SetTemporary();
  ScopedJNIObjectResult jresult(env);

  JNI_CALL_METHOD(env, handle_, "getResourceHandler",
                  "(Lorg/cef/browser/CefBrowser;Lorg/cef/browser/CefFrame;Lorg/"
                  "cef/network/CefRequest;)"
                  "Lorg/cef/handler/CefResourceHandler;",
                  Object, jresult, jbrowser.get(), jframe.get(),
                  jrequest.get());

  if (jresult)
    return new ResourceHandler(env, jresult);
  return nullptr;
}

void ResourceRequestHandler::OnResourceRedirect(CefRefPtr<CefBrowser> browser,
                                                CefRefPtr<CefFrame> frame,
                                                CefRefPtr<CefRequest> request,
                                                CefRefPtr<CefResponse> response,
                                                CefString& new_url) {
  ScopedJNIEnv env;
  if (!env)
    return;

  ScopedJNIBrowser jbrowser(env, browser);
  ScopedJNIFrame jframe(env, frame);
  jframe.SetTemporary();
  ScopedJNIRequest jrequest(env, request);
  jrequest.SetTemporary();
  ScopedJNIResponse jresponse(env, response);
  jresponse.SetTemporary();
  ScopedJNIStringRef jnewUrl(env, new_url);

  JNI_CALL_VOID_METHOD(
      env, handle_, "onResourceRedirect",
      "(Lorg/cef/browser/CefBrowser;Lorg/cef/browser/CefFrame;Lorg/cef/network/"
      "CefRequest;Lorg/cef/network/CefResponse;Lorg/cef/misc/"
      "StringRef;)V",
      jbrowser.get(), jframe.get(), jrequest.get(), jresponse.get(),
      jnewUrl.get());

  new_url = jnewUrl;
}

bool ResourceRequestHandler::OnResourceResponse(
    CefRefPtr<CefBrowser> browser,
    CefRefPtr<CefFrame> frame,
    CefRefPtr<CefRequest> request,
    CefRefPtr<CefResponse> response) {
  ScopedJNIEnv env;
  if (!env)
    return false;

  ScopedJNIBrowser jbrowser(env, browser);
  ScopedJNIFrame jframe(env, frame);
  jframe.SetTemporary();
  ScopedJNIRequest jrequest(env, request);
  jrequest.SetTemporary();
  ScopedJNIResponse jresponse(env, response);
  jresponse.SetTemporary();
  jboolean result = JNI_FALSE;

  JNI_CALL_METHOD(env, handle_, "onResourceResponse",
                  "(Lorg/cef/browser/CefBrowser;Lorg/cef/browser/CefFrame;"
                  "Lorg/cef/network/CefRequest;Lorg/cef/network/CefResponse;)Z",
                  Boolean, result, jbrowser.get(), jframe.get(), jrequest.get(),
                  jresponse.get());

  return (result != JNI_FALSE);
}

void ResourceRequestHandler::OnResourceLoadComplete(
    CefRefPtr<CefBrowser> browser,
    CefRefPtr<CefFrame> frame,
    CefRefPtr<CefRequest> request,
    CefRefPtr<CefResponse> response,
    CefResourceRequestHandler::URLRequestStatus status,
    int64_t received_content_length) {
  ScopedJNIEnv env;
  if (!env)
    return;

  ScopedJNIBrowser jbrowser(env, browser);
  ScopedJNIFrame jframe(env, frame);
  jframe.SetTemporary();
  ScopedJNIRequest jrequest(env, request);
  jrequest.SetTemporary();
  ScopedJNIResponse jresponse(env, response);
  jresponse.SetTemporary();
  ScopedJNIURLRequestStatus jstatus(env, status);

  JNI_CALL_VOID_METHOD(
      env, handle_, "onResourceLoadComplete",
      "(Lorg/cef/browser/CefBrowser;Lorg/cef/browser/CefFrame;"
      "Lorg/cef/network/CefRequest;Lorg/cef/network/CefResponse;"
      "Lorg/cef/network/CefURLRequest$Status;J)V",
      jbrowser.get(), jframe.get(), jrequest.get(), jresponse.get(),
      jstatus.get(), (jlong)received_content_length);
}

void ResourceRequestHandler::OnProtocolExecution(CefRefPtr<CefBrowser> browser,
                                                 CefRefPtr<CefFrame> frame,
                                                 CefRefPtr<CefRequest> request,
                                                 bool& allow_os_execution) {
  ScopedJNIEnv env;
  if (!env)
    return;

  ScopedJNIBrowser jbrowser(env, browser);
  ScopedJNIFrame jframe(env, frame);
  jframe.SetTemporary();
  ScopedJNIRequest jrequest(env, request);
  jrequest.SetTemporary();
  ScopedJNIBoolRef jallowOsExecution(env, allow_os_execution);

  JNI_CALL_VOID_METHOD(
      env, handle_, "onProtocolExecution",
      "(Lorg/cef/browser/CefBrowser;Lorg/cef/browser/CefFrame;Lorg/cef/network/"
      "CefRequest;Lorg/cef/misc/BoolRef;)V",
      jbrowser.get(), jframe.get(), jrequest.get(), jallowOsExecution.get());

  allow_os_execution = jallowOsExecution;
}
