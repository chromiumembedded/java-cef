// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "request_handler.h"

#include "client_handler.h"
#include "jni_util.h"
#include "resource_handler.h"
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
                                    bool user_gesture,
                                    bool is_redirect) {
  // forward request to clienthandler to make the message_router_ happy
  CefRefPtr<ClientHandler> client =
      (ClientHandler*)browser->GetHost()->GetClient().get();
  client->OnBeforeBrowse(browser, frame);

  JNIEnv* env = GetJNIEnv();
  if (!env)
    return false;

  jobject jframe = GetJNIFrame(env, frame);
  if (!jframe)
    return false;

  jobject jrequest = NewJNIObject(env, "org/cef/network/CefRequest_N");
  if (!jrequest) {
    return false;
    env->DeleteLocalRef(jframe);
  }
  SetCefForJNIObject(env, jrequest, request.get(), "CefRequest");

  jboolean result = JNI_FALSE;
  jobject jbrowser = GetJNIBrowser(browser);
  JNI_CALL_METHOD(env, jhandler_, "onBeforeBrowse",
                  "(Lorg/cef/browser/CefBrowser;Lorg/cef/browser/CefFrame;Lorg/"
                  "cef/network/CefRequest;ZZ)Z",
                  Boolean, result, jbrowser, jframe, jrequest,
                  (user_gesture ? JNI_TRUE : JNI_FALSE),
                  (is_redirect ? JNI_TRUE : JNI_FALSE));

  SetCefForJNIObject<CefRequest>(env, jframe, NULL, "CefFrame");
  SetCefForJNIObject<CefRequest>(env, jrequest, NULL, "CefRequest");
  env->DeleteLocalRef(jbrowser);
  env->DeleteLocalRef(jframe);
  env->DeleteLocalRef(jrequest);
  return (result != JNI_FALSE);
}

// TODO(JCEF): Expose the |callback| parameter.
RequestHandler::ReturnValue RequestHandler::OnBeforeResourceLoad(
    CefRefPtr<CefBrowser> browser,
    CefRefPtr<CefFrame> frame,
    CefRefPtr<CefRequest> request,
    CefRefPtr<CefRequestCallback> callback) {
  JNIEnv* env = GetJNIEnv();
  if (!env)
    return RV_CONTINUE;

  jobject jframe = GetJNIFrame(env, frame);
  if (!jframe)
    return RV_CONTINUE;

  jobject jrequest = NewJNIObject(env, "org/cef/network/CefRequest_N");
  if (!jrequest) {
    return RV_CONTINUE;
    env->DeleteLocalRef(jframe);
  }
  SetCefForJNIObject(env, jrequest, request.get(), "CefRequest");

  jboolean result = JNI_FALSE;
  jobject jbrowser = GetJNIBrowser(browser);
  JNI_CALL_METHOD(env, jhandler_, "onBeforeResourceLoad",
                  "(Lorg/cef/browser/CefBrowser;Lorg/cef/browser/CefFrame;Lorg/"
                  "cef/network/CefRequest;)Z",
                  Boolean, result, jbrowser, jframe, jrequest);

  SetCefForJNIObject<CefRequest>(env, jframe, NULL, "CefFrame");
  SetCefForJNIObject<CefRequest>(env, jrequest, NULL, "CefRequest");
  env->DeleteLocalRef(jbrowser);
  env->DeleteLocalRef(jframe);
  env->DeleteLocalRef(jrequest);
  return (result != JNI_FALSE) ? RV_CANCEL : RV_CONTINUE;
}

CefRefPtr<CefResourceHandler> RequestHandler::GetResourceHandler(
    CefRefPtr<CefBrowser> browser,
    CefRefPtr<CefFrame> frame,
    CefRefPtr<CefRequest> request) {
  JNIEnv* env = GetJNIEnv();
  if (!env)
    return NULL;

  jobject jframe = GetJNIFrame(env, frame);
  if (!jframe)
    return NULL;

  jobject jrequest = NewJNIObject(env, "org/cef/network/CefRequest_N");
  if (!jrequest) {
    env->DeleteLocalRef(jframe);
    return NULL;
  }
  SetCefForJNIObject(env, jrequest, request.get(), "CefRequest");

  jobject jhandler = NULL;
  jobject jbrowser = GetJNIBrowser(browser);
  JNI_CALL_METHOD(env, jhandler_, "getResourceHandler",
                  "(Lorg/cef/browser/CefBrowser;Lorg/cef/browser/CefFrame;Lorg/"
                  "cef/network/CefRequest;)"
                  "Lorg/cef/handler/CefResourceHandler;",
                  Object, jhandler, jbrowser, jframe, jrequest);
  env->DeleteLocalRef(jbrowser);
  if (!jhandler) {
    env->DeleteLocalRef(jframe);
    env->DeleteLocalRef(jrequest);
    return NULL;
  }
  CefRefPtr<CefResourceHandler> handler = NULL;
  handler = GetCefFromJNIObject<CefResourceHandler>(env, jhandler,
                                                    "CefResourceHandler");
  if (!handler.get()) {
    handler = new ResourceHandler(env, jhandler);
    SetCefForJNIObject(env, jhandler, handler.get(), "CefResourceHandler");
  }
  env->DeleteLocalRef(jframe);
  env->DeleteLocalRef(jrequest);
  env->DeleteLocalRef(jhandler);
  return handler;
}

void RequestHandler::OnResourceRedirect(CefRefPtr<CefBrowser> browser,
                                        CefRefPtr<CefFrame> frame,
                                        CefRefPtr<CefRequest> request,
                                        CefRefPtr<CefResponse> response,
                                        CefString& new_url) {
  JNIEnv* env = GetJNIEnv();
  if (!env)
    return;

  jobject jframe = GetJNIFrame(env, frame);
  if (!jframe)
    return;

  jobject jrequest = NewJNIObject(env, "org/cef/network/CefRequest_N");
  if (!jrequest) {
    env->DeleteLocalRef(jframe);
    return;
  }
  SetCefForJNIObject(env, jrequest, request.get(), "CefRequest");

  jobject jresponse = NewJNIObject(env, "org/cef/network/CefResponse_N");
  if (!jresponse) {
    env->DeleteLocalRef(jrequest);
    env->DeleteLocalRef(jframe);
    return;
  }
  SetCefForJNIObject(env, jresponse, response.get(), "CefResponse");

  jobject jstringRef = NewJNIStringRef(env, new_url);
  jobject jbrowser = GetJNIBrowser(browser);
  JNI_CALL_VOID_METHOD(
      env, jhandler_, "onResourceRedirect",
      "(Lorg/cef/browser/CefBrowser;Lorg/cef/browser/CefFrame;Lorg/cef/network/"
      "CefRequest;Lorg/cef/network/CefResponse;Lorg/cef/misc/"
      "StringRef;)V",
      jbrowser, jframe, jrequest, jresponse, jstringRef);
  new_url = GetJNIStringRef(env, jstringRef);
  env->DeleteLocalRef(jbrowser);
  env->DeleteLocalRef(jresponse);
  env->DeleteLocalRef(jrequest);
  env->DeleteLocalRef(jframe);
  env->DeleteLocalRef(jstringRef);
}

bool RequestHandler::OnResourceResponse(CefRefPtr<CefBrowser> browser,
                                        CefRefPtr<CefFrame> frame,
                                        CefRefPtr<CefRequest> request,
                                        CefRefPtr<CefResponse> response) {
  JNIEnv* env = GetJNIEnv();
  if (!env)
    return false;

  jobject jframe = GetJNIFrame(env, frame);
  if (!jframe)
    return false;

  jobject jrequest = NewJNIObject(env, "org/cef/network/CefRequest_N");
  if (!jrequest) {
    env->DeleteLocalRef(jframe);
    return false;
  }
  SetCefForJNIObject(env, jrequest, request.get(), "CefRequest");

  jobject jresponse = NewJNIObject(env, "org/cef/network/CefResponse_N");
  if (!jresponse) {
    SetCefForJNIObject<CefRequest>(env, jrequest, NULL, "CefRequest");
    env->DeleteLocalRef(jframe);
    env->DeleteLocalRef(jrequest);
    return false;
  }
  SetCefForJNIObject(env, jresponse, response.get(), "CefResponse");

  jboolean result = JNI_FALSE;
  jobject jbrowser = GetJNIBrowser(browser);
  JNI_CALL_METHOD(env, jhandler_, "onResourceResponse",
                  "(Lorg/cef/browser/CefBrowser;Lorg/cef/browser/CefFrame;"
                  "Lorg/cef/network/CefRequest;Lorg/cef/network/CefResponse;)Z",
                  Boolean, result, jbrowser, jframe, jrequest,
                  jresponse);

  SetCefForJNIObject<CefRequest>(env, jframe, NULL, "CefFrame");
  SetCefForJNIObject<CefRequest>(env, jrequest, NULL, "CefRequest");
  SetCefForJNIObject<CefRequest>(env, jresponse, NULL, "CefResponse");
  env->DeleteLocalRef(jbrowser);
  env->DeleteLocalRef(jframe);
  env->DeleteLocalRef(jrequest);
  env->DeleteLocalRef(jresponse);
  return (result != JNI_FALSE);
}

void RequestHandler::OnResourceLoadComplete(
    CefRefPtr<CefBrowser> browser,
    CefRefPtr<CefFrame> frame,
    CefRefPtr<CefRequest> request,
    CefRefPtr<CefResponse> response,
    CefRequestHandler::URLRequestStatus status,
    int64 received_content_length) {
  JNIEnv* env = GetJNIEnv();
  if (!env)
    return;

  jobject jframe = GetJNIFrame(env, frame);
  if (!jframe)
    return;

  jobject jrequest = NewJNIObject(env, "org/cef/network/CefRequest_N");
  if (!jrequest) {
    env->DeleteLocalRef(jframe);
    return;
  }
  SetCefForJNIObject(env, jrequest, request.get(), "CefRequest");

  jobject jresponse = NewJNIObject(env, "org/cef/network/CefResponse_N");
  if (!jresponse) {
    SetCefForJNIObject<CefRequest>(env, jrequest, NULL, "CefRequest");
    env->DeleteLocalRef(jframe);
    env->DeleteLocalRef(jrequest);
    return;
  }
  SetCefForJNIObject(env, jresponse, response.get(), "CefResponse");

  jobject jstatus = NewJNIURLRequestStatus(env, status);
  if (!jstatus) {
    env->DeleteLocalRef(jresponse);
    env->DeleteLocalRef(jrequest);
    env->DeleteLocalRef(jframe);
    return;
  }

  jobject jbrowser = GetJNIBrowser(browser);
  JNI_CALL_VOID_METHOD(
      env, jhandler_, "onResourceLoadComplete",
      "(Lorg/cef/browser/CefBrowser;Lorg/cef/browser/CefFrame;"
      "Lorg/cef/network/CefRequest;Lorg/cef/network/CefResponse;"
      "Lorg/cef/network/CefURLRequest$Status;J)V",
      jbrowser, jframe, jrequest, jresponse, jstatus,
      (jlong)received_content_length);

  SetCefForJNIObject<CefRequest>(env, jframe, NULL, "CefFrame");
  SetCefForJNIObject<CefRequest>(env, jrequest, NULL, "CefRequest");
  SetCefForJNIObject<CefRequest>(env, jresponse, NULL, "CefResponse");
  env->DeleteLocalRef(jbrowser);
  env->DeleteLocalRef(jframe);
  env->DeleteLocalRef(jrequest);
  env->DeleteLocalRef(jresponse);
  env->DeleteLocalRef(jstatus);
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

  jobject jframe = GetJNIFrame(env, frame);
  if (!jframe)
    return false;

  jobject jcallback = NewJNIObject(env, "org/cef/callback/CefAuthCallback_N");
  if (!jcallback) {
    env->DeleteLocalRef(jframe);
    return false;
  }
  SetCefForJNIObject(env, jcallback, callback.get(), "CefAuthCallback");

  jboolean result = JNI_FALSE;
  jstring jhost = NewJNIString(env, host);
  jstring jrealm = NewJNIString(env, realm);
  jstring jscheme = NewJNIString(env, scheme);
  jobject jbrowser = GetJNIBrowser(browser);
  JNI_CALL_METHOD(env, jhandler_, "getAuthCredentials",
                  "(Lorg/cef/browser/CefBrowser;Lorg/cef/browser/"
                  "CefFrame;ZLjava/lang/String;"
                  "ILjava/lang/String;Ljava/lang/String;"
                  "Lorg/cef/callback/CefAuthCallback;)Z",
                  Boolean, result, jbrowser, jframe,
                  (isProxy ? JNI_TRUE : JNI_FALSE), jhost, port, jrealm,
                  jscheme, jcallback);

  if (result == JNI_FALSE) {
    // If the java method returns "false", the callback won't be used and
    // therefore the reference can be removed.
    SetCefForJNIObject<CefAuthCallback>(env, jcallback, NULL,
                                        "CefAuthCallback");
  }

  SetCefForJNIObject<CefRequest>(env, jframe, NULL, "CefFrame");
  env->DeleteLocalRef(jbrowser);
  env->DeleteLocalRef(jcallback);
  env->DeleteLocalRef(jframe);
  env->DeleteLocalRef(jhost);
  env->DeleteLocalRef(jrealm);
  env->DeleteLocalRef(jscheme);
  return (result != JNI_FALSE);
}

bool RequestHandler::OnQuotaRequest(CefRefPtr<CefBrowser> browser,
                                    const CefString& origin_url,
                                    int64 new_size,
                                    CefRefPtr<CefRequestCallback> callback) {
  JNIEnv* env = GetJNIEnv();
  if (!env)
    return false;

  jobject jcallback =
      NewJNIObject(env, "org/cef/callback/CefRequestCallback_N");
  if (!jcallback)
    return false;
  SetCefForJNIObject(env, jcallback, callback.get(), "CefRequestCallback");

  jboolean result = JNI_FALSE;
  jstring jorigin_url = NewJNIString(env, origin_url);
  jobject jbrowser = GetJNIBrowser(browser);
  JNI_CALL_METHOD(env, jhandler_, "onQuotaRequest",
                  "(Lorg/cef/browser/CefBrowser;Ljava/lang/String;"
                  "JLorg/cef/callback/CefRequestCallback;)Z",
                  Boolean, result, jbrowser, jorigin_url,
                  (jlong)new_size, jcallback);

  if (result == JNI_FALSE) {
    // If the java method returns "false", the callback won't be used and
    // therefore the reference can be removed.
    SetCefForJNIObject<CefRequestCallback>(env, jcallback, NULL,
                                           "CefRequestCallback");
  }
  env->DeleteLocalRef(jbrowser);
  env->DeleteLocalRef(jcallback);
  env->DeleteLocalRef(jorigin_url);
  return (result != JNI_FALSE);
}

void RequestHandler::OnProtocolExecution(CefRefPtr<CefBrowser> browser,
                                         const CefString& url,
                                         bool& allow_os_execution) {
  JNIEnv* env = GetJNIEnv();
  if (!env)
    return;

  jobject jboolRef = NewJNIBoolRef(env, allow_os_execution);
  jstring jurl = NewJNIString(env, url);
  jobject jbrowser = GetJNIBrowser(browser);
  JNI_CALL_VOID_METHOD(
      env, jhandler_, "onProtocolExecution",
      "(Lorg/cef/browser/CefBrowser;Ljava/lang/String;Lorg/cef/misc/BoolRef;)V",
      jbrowser, jurl, jboolRef);
  allow_os_execution = GetJNIBoolRef(env, jboolRef);
  env->DeleteLocalRef(jbrowser);
  env->DeleteLocalRef(jboolRef);
  env->DeleteLocalRef(jurl);
}

bool RequestHandler::OnCertificateError(
    CefRefPtr<CefBrowser> browser,
    cef_errorcode_t cert_error,
    const CefString& request_url,
    CefRefPtr<CefSSLInfo> ssl_info,
    CefRefPtr<CefRequestCallback> callback) {
  JNIEnv* env = GetJNIEnv();
  if (!env)
    return false;

  jobject jcallback =
      NewJNIObject(env, "org/cef/callback/CefRequestCallback_N");
  if (!jcallback)
    return false;
  SetCefForJNIObject(env, jcallback, callback.get(), "CefRequestCallback");

  jboolean result = JNI_FALSE;
  jstring jrequest_url = NewJNIString(env, request_url);
  jobject jbrowser = GetJNIBrowser(browser);
  jobject jerrorcode = NewJNIErrorCode(env, cert_error);
  JNI_CALL_METHOD(
      env, jhandler_, "onCertificateError",
      "(Lorg/cef/browser/CefBrowser;Lorg/cef/handler/CefLoadHandler$ErrorCode;"
      "Ljava/lang/String;Lorg/cef/callback/CefRequestCallback;)Z",
      Boolean, result, jbrowser, jerrorcode,
      jrequest_url, jcallback);

  if (result == JNI_FALSE) {
    // If the java method returns "false", the callback won't be used and
    // therefore the reference can be removed.
    SetCefForJNIObject<CefRequestCallback>(env, jcallback, NULL,
                                           "CefRequestCallback");
  }
  env->DeleteLocalRef(jbrowser);
  env->DeleteLocalRef(jerrorcode);
  env->DeleteLocalRef(jrequest_url);
  env->DeleteLocalRef(jcallback);
  return (result != JNI_FALSE);
}

void RequestHandler::OnPluginCrashed(CefRefPtr<CefBrowser> browser,
                                     const CefString& plugin_path) {
  JNIEnv* env = GetJNIEnv();
  if (!env)
    return;

  jstring jplugin_path = NewJNIString(env, plugin_path);
  jobject jbrowser = GetJNIBrowser(browser);
  JNI_CALL_VOID_METHOD(env, jhandler_, "onPluginCrashed",
                       "(Lorg/cef/browser/CefBrowser;Ljava/lang/String;)V",
                       jbrowser, jplugin_path);
  env->DeleteLocalRef(jbrowser);
  env->DeleteLocalRef(jplugin_path);
}

void RequestHandler::OnRenderProcessTerminated(CefRefPtr<CefBrowser> browser,
                                               TerminationStatus status) {
  // forward request to clienthandler to make the message_router_ happy
  CefRefPtr<ClientHandler> client =
      (ClientHandler*)browser->GetHost()->GetClient().get();
  client->OnRenderProcessTerminated(browser);

  JNIEnv* env = GetJNIEnv();
  if (!env)
    return;

  jobject jstatus = NULL;
  switch (status) {
    JNI_CASE(env, "org/cef/handler/CefRequestHandler$TerminationStatus",
             TS_ABNORMAL_TERMINATION, jstatus);
    JNI_CASE(env, "org/cef/handler/CefRequestHandler$TerminationStatus",
             TS_PROCESS_WAS_KILLED, jstatus);
    JNI_CASE(env, "org/cef/handler/CefRequestHandler$TerminationStatus",
             TS_PROCESS_CRASHED, jstatus);
    JNI_CASE(env, "org/cef/handler/CefRequestHandler$TerminationStatus",
             TS_PROCESS_OOM, jstatus);
  }

  jobject jbrowser = GetJNIBrowser(browser);
  JNI_CALL_VOID_METHOD(
      env, jhandler_, "onRenderProcessTerminated",
      "(Lorg/cef/browser/CefBrowser;"
      "Lorg/cef/handler/CefRequestHandler$TerminationStatus;)V",
      jbrowser, jstatus);
  env->DeleteLocalRef(jbrowser);
  env->DeleteLocalRef(jstatus);
}
