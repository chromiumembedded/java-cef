// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "load_handler.h"
#include "client_handler.h"

#include "jni_util.h"
#include "util.h"

LoadHandler::LoadHandler(JNIEnv* env, jobject handler) {
  jhandler_ = env->NewGlobalRef(handler);
}

LoadHandler::~LoadHandler() {
  JNIEnv* env = GetJNIEnv();
  env->DeleteGlobalRef(jhandler_);
}

void LoadHandler::OnLoadingStateChange(CefRefPtr<CefBrowser> browser,
                                       bool isLoading,
                                       bool canGoBack,
                                       bool canGoForward) {
  JNIEnv* env = GetJNIEnv();
  if (!env)
    return;
  JNI_CALL_VOID_METHOD(env, jhandler_,
                       "onLoadingStateChange",
                       "(Lorg/cef/browser/CefBrowser;ZZZ)V",
                       GetJNIBrowser(browser),
                       (isLoading ? JNI_TRUE : JNI_FALSE),
                       (canGoBack ? JNI_TRUE : JNI_FALSE),
                       (canGoForward ? JNI_TRUE : JNI_FALSE));
}

// TODO(jcef): Expose the |transition_type| argument.
void LoadHandler::OnLoadStart(CefRefPtr<CefBrowser> browser,
                              CefRefPtr<CefFrame> frame,
                              TransitionType transition_type) {
  JNIEnv* env = GetJNIEnv();
  if (!env)
    return;
  JNI_CALL_VOID_METHOD(env, jhandler_,
                       "onLoadStart",
                       "(Lorg/cef/browser/CefBrowser;I)V",
                       GetJNIBrowser(browser),
                       frame->GetIdentifier());
}

void LoadHandler::OnLoadEnd(CefRefPtr<CefBrowser> browser,
                            CefRefPtr<CefFrame> frame,
                            int httpStatusCode) {
  JNIEnv* env = GetJNIEnv();
  if (!env)
    return;
  JNI_CALL_VOID_METHOD(env, jhandler_,
                       "onLoadEnd",
                       "(Lorg/cef/browser/CefBrowser;II)V",
                       GetJNIBrowser(browser),
                       frame->GetIdentifier(),
                       httpStatusCode);
}

void LoadHandler::OnLoadError(CefRefPtr<CefBrowser> browser,
                              CefRefPtr<CefFrame> frame,
                              ErrorCode errorCode,
                              const CefString& errorText,
                              const CefString& failedUrl) {
  JNIEnv* env = GetJNIEnv();
  if (!env)
    return;
  JNI_CALL_VOID_METHOD(env, jhandler_,
                       "onLoadError",
                       "(Lorg/cef/browser/CefBrowser;ILorg/cef/handler/CefLoadHandler$ErrorCode;Ljava/lang/String;Ljava/lang/String;)V",
                       GetJNIBrowser(browser),
                       frame->GetIdentifier(),
                       NewJNIErrorCode(env, errorCode),
                       NewJNIString(env, errorText),
                       NewJNIString(env, failedUrl));
}
