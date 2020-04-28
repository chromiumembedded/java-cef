// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "load_handler.h"

#include "client_handler.h"
#include "jni_util.h"
#include "util.h"

LoadHandler::LoadHandler(JNIEnv* env, jobject handler)
    : handle_(env, handler) {}

void LoadHandler::OnLoadingStateChange(CefRefPtr<CefBrowser> browser,
                                       bool isLoading,
                                       bool canGoBack,
                                       bool canGoForward) {
  ScopedJNIEnv env;
  if (!env)
    return;

  ScopedJNIBrowser jbrowser(env, browser);

  JNI_CALL_VOID_METHOD(env, handle_, "onLoadingStateChange",
                       "(Lorg/cef/browser/CefBrowser;ZZZ)V", jbrowser.get(),
                       (isLoading ? JNI_TRUE : JNI_FALSE),
                       (canGoBack ? JNI_TRUE : JNI_FALSE),
                       (canGoForward ? JNI_TRUE : JNI_FALSE));
}

// TODO(jcef): Expose the |transition_type| argument.
void LoadHandler::OnLoadStart(CefRefPtr<CefBrowser> browser,
                              CefRefPtr<CefFrame> frame,
                              TransitionType transition_type) {
  ScopedJNIEnv env;
  if (!env)
    return;

  ScopedJNIBrowser jbrowser(env, browser);
  ScopedJNIFrame jframe(env, frame);
  jframe.SetTemporary();
  ScopedJNITransitionType jtransitionType(env, transition_type);

  JNI_CALL_VOID_METHOD(env, handle_, "onLoadStart",
                       "(Lorg/cef/browser/CefBrowser;Lorg/cef/browser/"
                       "CefFrame;Lorg/cef/network/CefRequest$TransitionType;)V",
                       jbrowser.get(), jframe.get(), jtransitionType.get());
}

void LoadHandler::OnLoadEnd(CefRefPtr<CefBrowser> browser,
                            CefRefPtr<CefFrame> frame,
                            int httpStatusCode) {
  ScopedJNIEnv env;
  if (!env)
    return;

  ScopedJNIBrowser jbrowser(env, browser);
  ScopedJNIFrame jframe(env, frame);
  jframe.SetTemporary();

  JNI_CALL_VOID_METHOD(
      env, handle_, "onLoadEnd",
      "(Lorg/cef/browser/CefBrowser;Lorg/cef/browser/CefFrame;I)V",
      jbrowser.get(), jframe.get(), httpStatusCode);
}

void LoadHandler::OnLoadError(CefRefPtr<CefBrowser> browser,
                              CefRefPtr<CefFrame> frame,
                              ErrorCode errorCode,
                              const CefString& errorText,
                              const CefString& failedUrl) {
  ScopedJNIEnv env;
  if (!env)
    return;

  ScopedJNIBrowser jbrowser(env, browser);
  ScopedJNIFrame jframe(env, frame);
  jframe.SetTemporary();
  ScopedJNIObjectLocal jErrorCode(env, NewJNIErrorCode(env, errorCode));
  ScopedJNIString jerrorText(env, errorText);
  ScopedJNIString jfailedUrl(env, failedUrl);

  JNI_CALL_VOID_METHOD(
      env, handle_, "onLoadError",
      "(Lorg/cef/browser/CefBrowser;Lorg/cef/browser/CefFrame;Lorg/cef/handler/"
      "CefLoadHandler$ErrorCode;Ljava/lang/String;Ljava/lang/String;)V",
      jbrowser.get(), jframe.get(), jErrorCode.get(), jerrorText.get(),
      jfailedUrl.get());
}
