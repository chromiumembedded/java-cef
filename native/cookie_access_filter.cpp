// Copyright (c) 2019 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "cookie_access_filter.h"

#include "jni_util.h"
#include "util.h"

CookieAccessFilter::CookieAccessFilter(JNIEnv* env, jobject handler)
    : handle_(env, handler) {}

bool CookieAccessFilter::CanSendCookie(CefRefPtr<CefBrowser> browser,
                                       CefRefPtr<CefFrame> frame,
                                       CefRefPtr<CefRequest> request,
                                       const CefCookie& cookie) {
  ScopedJNIEnv env;
  if (!env)
    return true;

  ScopedJNIBrowser jbrowser(env, browser);
  ScopedJNIFrame jframe(env, frame);
  jframe.SetTemporary();
  ScopedJNIRequest jrequest(env, request);
  jrequest.SetTemporary();
  ScopedJNICookie jcookie(env, cookie);
  jboolean result = JNI_TRUE;

  JNI_CALL_METHOD(env, handle_, "canSendCookie",
                  "(Lorg/cef/browser/CefBrowser;Lorg/cef/browser/CefFrame;Lorg/"
                  "cef/network/CefRequest;Lorg/cef/network/CefCookie;)Z",
                  Boolean, result, jbrowser.get(), jframe.get(), jrequest.get(),
                  jcookie.get());

  return (result != JNI_FALSE);
}

bool CookieAccessFilter::CanSaveCookie(CefRefPtr<CefBrowser> browser,
                                       CefRefPtr<CefFrame> frame,
                                       CefRefPtr<CefRequest> request,
                                       CefRefPtr<CefResponse> response,
                                       const CefCookie& cookie) {
  ScopedJNIEnv env;
  if (!env)
    return true;

  ScopedJNIBrowser jbrowser(env, browser);
  ScopedJNIFrame jframe(env, frame);
  jframe.SetTemporary();
  ScopedJNIRequest jrequest(env, request);
  jrequest.SetTemporary();
  ScopedJNIResponse jresponse(env, response);
  jresponse.SetTemporary();
  ScopedJNICookie jcookie(env, cookie);
  jboolean result = JNI_TRUE;

  JNI_CALL_METHOD(
      env, handle_, "canSaveCookie",
      "(Lorg/cef/browser/CefBrowser;Lorg/cef/browser/CefFrame;Lorg/cef/network/"
      "CefRequest;Lorg/cef/network/CefResponse;Lorg/cef/network/CefCookie;)Z",
      Boolean, result, jbrowser.get(), jframe.get(), jrequest.get(),
      jresponse.get(), jcookie.get());

  return (result != JNI_FALSE);
}
