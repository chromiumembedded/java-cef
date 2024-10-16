// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "request_handler.h"

#include "client_handler.h"
#include "jni_util.h"
#include "resource_handler.h"
#include "resource_request_handler.h"
#include "util.h"

RequestHandler::RequestHandler(JNIEnv* env, jobject handler)
    : handle_(env, handler) {}

bool RequestHandler::OnBeforeBrowse(CefRefPtr<CefBrowser> browser,
                                    CefRefPtr<CefFrame> frame,
                                    CefRefPtr<CefRequest> request,
                                    bool user_gesture,
                                    bool is_redirect) {
  // Forward request to ClientHandler to make the message_router_ happy.
  CefRefPtr<ClientHandler> client =
      (ClientHandler*)browser->GetHost()->GetClient().get();
  client->OnBeforeBrowse(browser, frame);

  ScopedJNIEnv env;
  if (!env)
    return false;

  ScopedJNIBrowser jbrowser(env, browser);
  ScopedJNIFrame jframe(env, frame);
  jframe.SetTemporary();
  ScopedJNIRequest jrequest(env, request);
  jrequest.SetTemporary();
  jboolean jresult = JNI_FALSE;

  JNI_CALL_METHOD(env, handle_, "onBeforeBrowse",
                  "(Lorg/cef/browser/CefBrowser;Lorg/cef/browser/CefFrame;Lorg/"
                  "cef/network/CefRequest;ZZ)Z",
                  Boolean, jresult, jbrowser.get(), jframe.get(),
                  jrequest.get(), (user_gesture ? JNI_TRUE : JNI_FALSE),
                  (is_redirect ? JNI_TRUE : JNI_FALSE));

  return (jresult != JNI_FALSE);
}

bool RequestHandler::OnOpenURLFromTab(CefRefPtr<CefBrowser> browser,
                                      CefRefPtr<CefFrame> frame,
                                      const CefString& target_url,
                                      WindowOpenDisposition target_disposition,
                                      bool user_gesture) {
  ScopedJNIEnv env;
  if (!env)
    return false;

  ScopedJNIBrowser jbrowser(env, browser);
  ScopedJNIFrame jframe(env, frame);
  jframe.SetTemporary();
  ScopedJNIString jtargetUrl(env, target_url);
  jboolean jresult = JNI_FALSE;

  JNI_CALL_METHOD(env, handle_, "onOpenURLFromTab",
                  "(Lorg/cef/browser/CefBrowser;Lorg/cef/browser/CefFrame;"
                  "Ljava/lang/String;Z)Z",
                  Boolean, jresult, jbrowser.get(), jframe.get(),
                  jtargetUrl.get(), (user_gesture ? JNI_TRUE : JNI_FALSE));

  return (jresult != JNI_FALSE);
}

CefRefPtr<CefResourceRequestHandler> RequestHandler::GetResourceRequestHandler(
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

bool RequestHandler::GetAuthCredentials(CefRefPtr<CefBrowser> browser,
                                        const CefString& origin_url,
                                        bool isProxy,
                                        const CefString& host,
                                        int port,
                                        const CefString& realm,
                                        const CefString& scheme,
                                        CefRefPtr<CefAuthCallback> callback) {
  ScopedJNIEnv env;
  if (!env)
    return false;

  ScopedJNIBrowser jbrowser(env, browser);
  ScopedJNIString joriginUrl(env, origin_url);
  ScopedJNIString jhost(env, host);
  ScopedJNIString jrealm(env, realm);
  ScopedJNIString jscheme(env, scheme);
  ScopedJNIAuthCallback jcallback(env, callback);
  jboolean jresult = JNI_FALSE;

  JNI_CALL_METHOD(
      env, handle_, "getAuthCredentials",
      "(Lorg/cef/browser/CefBrowser;Ljava/lang/String;ZLjava/lang/String;"
      "ILjava/lang/String;Ljava/lang/String;"
      "Lorg/cef/callback/CefAuthCallback;)Z",
      Boolean, jresult, jbrowser.get(), joriginUrl.get(),
      (isProxy ? JNI_TRUE : JNI_FALSE), jhost.get(), port, jrealm.get(),
      jscheme.get(), jcallback.get());

  if (jresult == JNI_FALSE) {
    // If the Java method returns "false" the callback won't be used and
    // the reference can therefore be removed.
    jcallback.SetTemporary();
  }

  return (jresult != JNI_FALSE);
}

bool RequestHandler::OnCertificateError(CefRefPtr<CefBrowser> browser,
                                        cef_errorcode_t cert_error,
                                        const CefString& request_url,
                                        CefRefPtr<CefSSLInfo> ssl_info,
                                        CefRefPtr<CefCallback> callback) {
  ScopedJNIEnv env;
  if (!env)
    return false;

  ScopedJNIBrowser jbrowser(env, browser);
  ScopedJNIObjectLocal jcertError(env, NewJNIErrorCode(env, cert_error));
  ScopedJNIString jrequestUrl(env, request_url);
  ScopedJNICallback jcallback(env, callback);
  jboolean jresult = JNI_FALSE;

  JNI_CALL_METHOD(
      env, handle_, "onCertificateError",
      "(Lorg/cef/browser/CefBrowser;Lorg/cef/handler/CefLoadHandler$ErrorCode;"
      "Ljava/lang/String;Lorg/cef/callback/CefCallback;)Z",
      Boolean, jresult, jbrowser.get(), jcertError.get(), jrequestUrl.get(),
      jcallback.get());

  if (jresult == JNI_FALSE) {
    // If the Java method returns "false" the callback won't be used and
    // the reference can therefore be removed.
    jcallback.SetTemporary();
  }

  return (jresult != JNI_FALSE);
}

void RequestHandler::OnRenderProcessTerminated(CefRefPtr<CefBrowser> browser,
                                               TerminationStatus status,
                                               int error_code,
                                               const CefString& error_string) {
  // Forward request to ClientHandler to make the message_router_ happy.
  CefRefPtr<ClientHandler> client =
      (ClientHandler*)browser->GetHost()->GetClient().get();
  client->OnRenderProcessTerminated(browser);

  ScopedJNIEnv env;
  if (!env)
    return;

  ScopedJNIBrowser jbrowser(env, browser);

  ScopedJNIObjectResult jstatus(env);
  switch (status) {
    JNI_CASE(env, "org/cef/handler/CefRequestHandler$TerminationStatus",
             TS_ABNORMAL_TERMINATION, jstatus);
    JNI_CASE(env, "org/cef/handler/CefRequestHandler$TerminationStatus",
             TS_PROCESS_WAS_KILLED, jstatus);
    JNI_CASE(env, "org/cef/handler/CefRequestHandler$TerminationStatus",
             TS_PROCESS_CRASHED, jstatus);
    JNI_CASE(env, "org/cef/handler/CefRequestHandler$TerminationStatus",
             TS_PROCESS_OOM, jstatus);
    JNI_CASE(env, "org/cef/handler/CefRequestHandler$TerminationStatus",
             TS_LAUNCH_FAILED, jstatus);
    JNI_CASE(env, "org/cef/handler/CefRequestHandler$TerminationStatus",
             TS_INTEGRITY_FAILURE, jstatus);
  }

  ScopedJNIString jerrorString(env, error_string);

  JNI_CALL_VOID_METHOD(
      env, handle_, "onRenderProcessTerminated",
      "(Lorg/cef/browser/CefBrowser;"
      "Lorg/cef/handler/CefRequestHandler$TerminationStatus;"
      "ILjava/lang/String;)V",
      jbrowser.get(), jstatus.get(), error_code, jerrorString.get());
}
