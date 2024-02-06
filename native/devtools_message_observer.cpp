// Copyright (c) 2024 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "devtools_message_observer.h"

#include "jni_util.h"

DevToolsMessageObserver::DevToolsMessageObserver(JNIEnv* env, jobject observer)
    : handle_(env, observer) {}

void DevToolsMessageObserver::OnDevToolsMethodResult(
    CefRefPtr<CefBrowser> browser,
    int message_id,
    bool success,
    const void* result,
    size_t result_size) {
  ScopedJNIEnv env;
  if (!env)
    return;

  ScopedJNIBrowser jbrowser(env, browser);
  std::string strResult(static_cast<const char*>(result), result_size);
  JNI_CALL_VOID_METHOD(
      env, handle_, "onDevToolsMethodResult",
      "(Lorg/cef/browser/CefBrowser;IZLjava/lang/String;)V", jbrowser.get(),
      message_id, success ? JNI_TRUE : JNI_FALSE, NewJNIString(env, strResult));
}

void DevToolsMessageObserver::OnDevToolsEvent(CefRefPtr<CefBrowser> browser,
                                              const CefString& method,
                                              const void* params,
                                              size_t params_size) {
  ScopedJNIEnv env;
  if (!env)
    return;
  ScopedJNIBrowser jbrowser(env, browser);

  std::string strParams(static_cast<const char*>(params), params_size);
  JNI_CALL_VOID_METHOD(
      env, handle_, "onDevToolsEvent",
      "(Lorg/cef/browser/CefBrowser;Ljava/lang/String;Ljava/lang/String;)V",
      jbrowser.get(), NewJNIString(env, method), NewJNIString(env, strParams));
}
