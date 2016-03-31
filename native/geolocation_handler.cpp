// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "geolocation_handler.h"
#include "client_handler.h"

#include "jni_util.h"
#include "util.h"

GeolocationHandler::GeolocationHandler(JNIEnv* env, jobject handler) {
  jhandler_ = env->NewGlobalRef(handler);
}

GeolocationHandler::~GeolocationHandler() {
  JNIEnv* env = GetJNIEnv();
  env->DeleteGlobalRef(jhandler_);
}

bool GeolocationHandler::OnRequestGeolocationPermission(
      CefRefPtr<CefBrowser> browser,
      const CefString& requesting_url,
      int request_id,
      CefRefPtr<CefGeolocationCallback> callback) {
  JNIEnv* env = GetJNIEnv();
  if (!env)
    return false;
  jobject jcallback =
      NewJNIObject(env, "org/cef/callback/CefGeolocationCallback_N");

  if (!jcallback)
    return false;
  SetCefForJNIObject(env, jcallback, callback.get(), "CefGeolocationCallback");
  jboolean jresult = JNI_FALSE;
  JNI_CALL_METHOD(env, jhandler_,
                  "onRequestGeolocationPermission",
                  "(Lorg/cef/browser/CefBrowser;Ljava/lang/String;"
                  "ILorg/cef/callback/CefGeolocationCallback;)Z",
                  Boolean,
                  jresult,
                  GetJNIBrowser(browser),
                  NewJNIString(env, requesting_url),
                  request_id,
                  jcallback);

  if (jresult == JNI_FALSE) {
    SetCefForJNIObject<CefGeolocationCallback>(env, jcallback, NULL,
        "CefGeolocationCallback");
  }
  return (jresult != JNI_FALSE);
}

void GeolocationHandler::OnCancelGeolocationPermission(
      CefRefPtr<CefBrowser> browser,
      int request_id) {
  JNIEnv* env = GetJNIEnv();
  if (!env)
    return;
  JNI_CALL_VOID_METHOD(env, jhandler_,
                       "onCancelGeolocationPermission",
                       "(Lorg/cef/browser/CefBrowser;I)V",
                       GetJNIBrowser(browser),
                       request_id);
}
