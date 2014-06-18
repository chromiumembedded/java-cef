// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "request_handler.h"
#include "resource_handler.h"
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

CefRefPtr<CefResourceHandler> RequestHandler::GetResourceHandler(
                                            CefRefPtr<CefBrowser> browser,
                                            CefRefPtr<CefFrame> frame,
                                            CefRefPtr<CefRequest> request) {
  JNIEnv* env = GetJNIEnv();
  if (!env)
    return NULL;

  jobject jrequest = NewJNIObject(env, "org/cef/network/CefRequest_N");
  if (!jrequest)
    return NULL;
  SetCefForJNIObject(env, jrequest, request.get(), "CefRequest");

  jobject jhandler = NULL;
  JNI_CALL_METHOD(env, jhandler_,
                  "getResourceHandler",
                  "(Lorg/cef/browser/CefBrowser;Lorg/cef/network/CefRequest;)"
                  "Lorg/cef/handler/CefResourceHandler;",
                  Object,
                  jhandler,
                  GetJNIBrowser(browser),
                  jrequest);
  if (!jhandler)
    return NULL;
  CefRefPtr<CefResourceHandler> handler = NULL;
  handler = GetCefFromJNIObject<CefResourceHandler>(env, jhandler, "CefResourceHandler");
  if (!handler.get()) {
    handler = new ResourceHandler(env, jhandler);
    SetCefForJNIObject(env, jhandler, handler.get(), "CefResourceHandler");
  }
  return handler;
}

void RequestHandler::OnResourceRedirect(CefRefPtr<CefBrowser> browser,
                                        CefRefPtr<CefFrame> frame,
                                        const CefString& old_url,
                                        CefString& new_url) {
  JNIEnv* env = GetJNIEnv();
  if (!env)
    return;
  jobject jstringRef = NewJNIStringRef(env, new_url);
  JNI_CALL_VOID_METHOD(env, jhandler_,
                       "onResourceRedirect",
                       "(Lorg/cef/browser/CefBrowser;Ljava/lang/String;Lorg/cef/misc/StringRef;)V",
                       GetJNIBrowser(browser),
                       NewJNIString(env, old_url),
                       jstringRef);
  new_url = GetJNIStringRef(env, jstringRef);
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

void RequestHandler::OnProtocolExecution(CefRefPtr<CefBrowser> browser,
                                         const CefString& url,
                                         bool& allow_os_execution) {
  JNIEnv* env = GetJNIEnv();
  if (!env)
    return;

  jobject jboolRef = NewJNIBoolRef(env, allow_os_execution);
  JNI_CALL_VOID_METHOD(env, jhandler_,
                       "onProtocolExecution",
                       "(Lorg/cef/browser/CefBrowser;Ljava/lang/String;Lorg/cef/misc/BoolRef;)V",
                       GetJNIBrowser(browser),
                       NewJNIString(env, url),
                       jboolRef);
  allow_os_execution = GetJNIBoolRef(env, jboolRef);
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

bool RequestHandler::OnBeforePluginLoad(CefRefPtr<CefBrowser> browser,
                                        const CefString& url,
                                        const CefString& policy_url,
                                        CefRefPtr<CefWebPluginInfo> info) {
  JNIEnv* env = GetJNIEnv();
  if (!env)
    return false;

  jobject jinfo = NewJNIObject(env, "org/cef/network/CefWebPluginInfo_N");
  if (!jinfo)
    return false;
  SetCefForJNIObject(env, jinfo, info.get(), "CefWebPluginInfo");

  jboolean jresult = JNI_FALSE;
  JNI_CALL_METHOD(env, jhandler_,
                       "onBeforePluginLoad",
                       "(Lorg/cef/browser/CefBrowser;Ljava/lang/String;"
                       "Ljava/lang/String;Lorg/cef/network/CefWebPluginInfo;)Z",
                       Boolean,
                       jresult,
                       GetJNIBrowser(browser),
                       NewJNIString(env, url),
                       NewJNIString(env, policy_url),
                       jinfo);

  SetCefForJNIObject<CefWebPluginInfo>(env, jinfo, NULL, "CefWebPluginInfo");
  return (jresult != JNI_FALSE);
}

void RequestHandler::OnPluginCrashed(CefRefPtr<CefBrowser> browser,
                                      const CefString& plugin_path) {
  JNIEnv* env = GetJNIEnv();
  if (!env)
    return;

  JNI_CALL_VOID_METHOD(env, jhandler_,
                            "onPluginCrashed",
                            "(Lorg/cef/browser/CefBrowser;Ljava/lang/String;)V",
                            GetJNIBrowser(browser),
                            NewJNIString(env, plugin_path));
}

void RequestHandler::OnRenderProcessTerminated(CefRefPtr<CefBrowser> browser,
                                               TerminationStatus status) {
  // forward request to clienthandler to make the message_router_ happy
  CefRefPtr<ClientHandler> client = (ClientHandler*)browser->GetHost()->GetClient().get();
  client->OnRenderProcessTerminated(browser);

  JNIEnv* env = GetJNIEnv();
  if (!env)
    return;

  jobject jstatus = NULL;
  switch (status) {
    JNI_CASE(env, "org/cef/handler/CefRequestHandler$TerminationStatus", TS_ABNORMAL_TERMINATION, jstatus);
    JNI_CASE(env, "org/cef/handler/CefRequestHandler$TerminationStatus", TS_PROCESS_WAS_KILLED, jstatus);
    JNI_CASE(env, "org/cef/handler/CefRequestHandler$TerminationStatus", TS_PROCESS_CRASHED, jstatus);
  }

  JNI_CALL_VOID_METHOD(env, jhandler_,
                            "onRenderProcessTerminated",
                            "(Lorg/cef/browser/CefBrowser;"
                            "Lorg/cef/handler/CefRequestHandler$TerminationStatus;)V",
                            GetJNIBrowser(browser),
                            jstatus);
}
