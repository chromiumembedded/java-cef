// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "message_router_handler.h"

#include "jni_util.h"

namespace {

using CefQueryCallback = CefMessageRouterBrowserSide::Callback;

// JNI CefQueryCallback object.
class ScopedJNIQueryCallback : public ScopedJNIObject<CefQueryCallback> {
 public:
  ScopedJNIQueryCallback(JNIEnv* env, CefRefPtr<CefQueryCallback> obj)
      : ScopedJNIObject<CefQueryCallback>(env,
                                          obj,
                                          "org/cef/callback/CefQueryCallback_N",
                                          "CefQueryCallback") {}
};

}  // namespace

MessageRouterHandler::MessageRouterHandler(JNIEnv* env, jobject handler)
    : handle_(env, handler) {}

bool MessageRouterHandler::OnQuery(
    CefRefPtr<CefBrowser> browser,
    CefRefPtr<CefFrame> frame,
    int64_t query_id,
    const CefString& request,
    bool persistent,
    CefRefPtr<CefMessageRouterBrowserSide::Callback> callback) {
  ScopedJNIEnv env;
  if (!env)
    return false;

  ScopedJNIBrowser jbrowser(env, browser);
  ScopedJNIFrame jframe(env, frame);
  jframe.SetTemporary();
  ScopedJNIString jrequest(env, request);
  ScopedJNIQueryCallback jcallback(env, callback);

  jboolean jresult = JNI_FALSE;

  JNI_CALL_METHOD(env, handle_, "onQuery",
                  "(Lorg/cef/browser/CefBrowser;Lorg/cef/browser/"
                  "CefFrame;JLjava/lang/String;ZLorg/cef/"
                  "callback/CefQueryCallback;)Z",
                  Boolean, jresult, jbrowser.get(), jframe.get(),
                  (jlong)query_id, jrequest.get(),
                  persistent ? JNI_TRUE : JNI_FALSE, jcallback.get());

  if (jresult == JNI_FALSE) {
    // If the Java method returns "false" the callback won't be used and
    // the reference can therefore be removed.
    jcallback.SetTemporary();
  }

  return jresult != JNI_FALSE;
}

void MessageRouterHandler::OnQueryCanceled(CefRefPtr<CefBrowser> browser,
                                           CefRefPtr<CefFrame> frame,
                                           int64_t query_id) {
  ScopedJNIEnv env;
  if (!env)
    return;

  ScopedJNIBrowser jbrowser(env, browser);
  ScopedJNIFrame jframe(env, frame);
  jframe.SetTemporary();

  JNI_CALL_VOID_METHOD(
      env, handle_, "onQueryCanceled",
      "(Lorg/cef/browser/CefBrowser;Lorg/cef/browser/CefFrame;J)V",
      jbrowser.get(), jframe.get(), (jlong)query_id);
}
