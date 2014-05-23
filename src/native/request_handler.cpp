// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "request_handler.h"
#include "client_handler.h"

#include "jni_util.h"
#include "util.h"

RequestHandler::RequestHandler(JNIEnv* env, jobject handler) {
  jhandler_ = env->NewGlobalRef(handler);
}

RequestHandler::~RequestHandler() {
  JNIEnv* env = GetJNIEnv();
  env->DeleteGlobalRef(jhandler_);
}

bool RequestHandler::OnBeforeBrowse(CefRefPtr<CefBrowser> browser,
                                    CefRefPtr<CefFrame> frame,
                                    CefRefPtr<CefRequest> request,
                                    bool is_redirect) {
  // forward request to clienthandler to make the message_router_ happy
  CefRefPtr<ClientHandler> client = (ClientHandler*)browser->GetHost()->GetClient().get();
  client->OnBeforeBrowse(browser, frame);

  JNIEnv* env = GetJNIEnv();
  if (!env)
    return false;

  jobject jrequest = NewJNIObject(env, "org/cef/network/CefRequest_N");
  if (!jrequest)
    return false;
  SetCefForJNIObject(env, jrequest, request.get(), "CefRequest");

  jboolean result = JNI_FALSE;
  JNI_CALL_METHOD(env, jhandler_,
                  "onBeforeBrowse",
                  "(Lorg/cef/browser/CefBrowser;Lorg/cef/network/CefRequest;Z)Z",
                  Boolean,
                  result,
                  GetJNIBrowser(browser),
                  jrequest,
                  (is_redirect ? JNI_TRUE : JNI_FALSE));

  SetCefForJNIObject<CefRequest>(env, jrequest, NULL, "CefRequest");
  return (result != JNI_FALSE);
}

bool RequestHandler::OnBeforeResourceLoad(CefRefPtr<CefBrowser> browser,
                                          CefRefPtr<CefFrame> frame,
                                          CefRefPtr<CefRequest> request) {
  JNIEnv* env = GetJNIEnv();
  if (!env)
    return false;

  jobject jrequest = NewJNIObject(env, "org/cef/network/CefRequest_N");
  if (!jrequest)
    return false;
  SetCefForJNIObject(env, jrequest, request.get(), "CefRequest");

  jboolean result = JNI_FALSE;
  JNI_CALL_METHOD(env, jhandler_,
                  "onBeforeResourceLoad",
                  "(Lorg/cef/browser/CefBrowser;Lorg/cef/network/CefRequest;)Z",
                  Boolean,
                  result,
                  GetJNIBrowser(browser),
                  jrequest);

  SetCefForJNIObject<CefRequest>(env, jrequest, NULL, "CefRequest");
  return (result != JNI_FALSE);
}

bool RequestHandler::GetAuthCredentials(CefRefPtr<CefBrowser> browser,
                                        CefRefPtr<CefFrame> frame,
                                        bool isProxy,
                                        const CefString& host,
                                        int port,
                                        const CefString& realm,
                                        const CefString& scheme,
                                        CefRefPtr<CefAuthCallback> callback) {
  JNIEnv* env = GetJNIEnv();
  if (!env)
    return false;

  jobject jcallback = NewJNIObject(env, "org/cef/callback/CefAuthCallback_N");
  if (!jcallback)
    return false;
  SetCefForJNIObject(env, jcallback, callback.get(), "CefAuthCallback");

  jboolean result = JNI_FALSE;
  JNI_CALL_METHOD(env, jhandler_,
                  "getAuthCredentials",
                  "(Lorg/cef/browser/CefBrowser;ZLjava/lang/String;"
                  "ILjava/lang/String;Ljava/lang/String;"
                  "Lorg/cef/callback/CefAuthCallback;)Z",
                  Boolean,
                  result,
                  GetJNIBrowser(browser),
                  (isProxy ? JNI_TRUE : JNI_FALSE),
                  NewJNIString(env, host),
                  port,
                  NewJNIString(env, realm),
                  NewJNIString(env, scheme),
                  jcallback);

  if (result == JNI_FALSE) {
    // If the java method returns "false", the callback won't be used and therefore
    // the reference can be removed.
    SetCefForJNIObject<CefAuthCallback>(env, jcallback, NULL, "CefAuthCallback");
  }
  return (result != JNI_FALSE);
}

bool RequestHandler::OnQuotaRequest(CefRefPtr<CefBrowser> browser,
                                    const CefString& origin_url,
                                    int64 new_size,
                                    CefRefPtr<CefQuotaCallback> callback) {
  JNIEnv* env = GetJNIEnv();
  if (!env)
    return false;

  jobject jcallback = NewJNIObject(env, "org/cef/callback/CefQuotaCallback_N");
  if (!jcallback)
    return false;
  SetCefForJNIObject(env, jcallback, callback.get(), "CefQuotaCallback");

  jboolean result = JNI_FALSE;
  JNI_CALL_METHOD(env, jhandler_,
                  "onQuotaRequest",
                  "(Lorg/cef/browser/CefBrowser;Ljava/lang/String;"
                  "JLorg/cef/callback/CefQuotaCallback;)Z",
                  Boolean,
                  result,
                  GetJNIBrowser(browser),
                  NewJNIString(env, origin_url),
                  (jlong)new_size,
                  jcallback);

  if (result == JNI_FALSE) {
    // If the java method returns "false", the callback won't be used and therefore
    // the reference can be removed.
    SetCefForJNIObject<CefQuotaCallback>(env, jcallback, NULL, "CefQuotaCallback");
  }
  return (result != JNI_FALSE);
}

bool RequestHandler::OnCertificateError(cef_errorcode_t cert_error,
                                        const CefString& request_url,
                                        CefRefPtr<CefAllowCertificateErrorCallback> callback) {
  JNIEnv* env = GetJNIEnv();
  if (!env)
    return false;

  jobject jcallback = NewJNIObject(env, "org/cef/callback/CefAllowCertificateErrorCallback_N");
  if (!jcallback)
    return false;
  SetCefForJNIObject(env, jcallback, callback.get(), "CefAllowCertificateErrorCallback");

  jboolean result = JNI_FALSE;
  JNI_CALL_METHOD(env, jhandler_,
                  "onCertificateError",
                  "(Lorg/cef/handler/CefLoadHandler$ErrorCode;Ljava/lang/String;"
                  "Lorg/cef/callback/CefAllowCertificateErrorCallback;)Z",
                  Boolean,
                  result,
                  NewJNIErrorCode(env, cert_error),
                  NewJNIString(env, request_url),
                  jcallback);

  if (result == JNI_FALSE) {
    // If the java method returns "false", the callback won't be used and therefore
    // the reference can be removed.
    SetCefForJNIObject<CefAllowCertificateErrorCallback>(env, jcallback, NULL, 
                                                         "CefAllowCertificateErrorCallback");
  }
  return (result != JNI_FALSE);
}

void RequestHandler::OnRenderProcessTerminated(CefRefPtr<CefBrowser> browser,
                                               TerminationStatus status) {
  // forward request to clienthandler to make the message_router_ happy
  CefRefPtr<ClientHandler> client = (ClientHandler*)browser->GetHost()->GetClient().get();
  client->OnRenderProcessTerminated(browser);
}
