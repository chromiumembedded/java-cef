// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "display_handler.h"

#include "client_handler.h"
#include "jni_util.h"
#include "util.h"

DisplayHandler::DisplayHandler(JNIEnv* env, jobject handler) {
  jhandler_ = env->NewGlobalRef(handler);
}

DisplayHandler::~DisplayHandler() {
  JNIEnv* env = GetJNIEnv();
  env->DeleteGlobalRef(jhandler_);
}

void DisplayHandler::OnAddressChange(CefRefPtr<CefBrowser> browser,
                                     CefRefPtr<CefFrame> frame,
                                     const CefString& url) {
  JNIEnv* env = GetJNIEnv();
  if (!env)
    return;
  JNI_CALL_VOID_METHOD(env, jhandler_, "onAddressChange",
                       "(Lorg/cef/browser/CefBrowser;Lorg/cef/browser/"
                       "CefFrame;Ljava/lang/String;)V",
                       GetJNIBrowser(browser), GetJNIFrame(env, frame),
                       NewJNIString(env, url));
}

void DisplayHandler::OnTitleChange(CefRefPtr<CefBrowser> browser,
                                   const CefString& title) {
  JNIEnv* env = GetJNIEnv();
  if (!env)
    return;
  JNI_CALL_VOID_METHOD(env, jhandler_, "onTitleChange",
                       "(Lorg/cef/browser/CefBrowser;Ljava/lang/String;)V",
                       GetJNIBrowser(browser), NewJNIString(env, title));
}

bool DisplayHandler::OnTooltip(CefRefPtr<CefBrowser> browser, CefString& text) {
  JNIEnv* env = GetJNIEnv();
  if (!env)
    return false;
  jboolean jreturn = JNI_FALSE;
  JNI_CALL_METHOD(env, jhandler_, "onTooltip",
                  "(Lorg/cef/browser/CefBrowser;Ljava/lang/String;)Z", Boolean,
                  jreturn, GetJNIBrowser(browser), NewJNIString(env, text));
  return (jreturn != JNI_FALSE);
}

void DisplayHandler::OnStatusMessage(CefRefPtr<CefBrowser> browser,
                                     const CefString& value) {
  JNIEnv* env = GetJNIEnv();
  if (!env)
    return;
  JNI_CALL_VOID_METHOD(env, jhandler_, "onStatusMessage",
                       "(Lorg/cef/browser/CefBrowser;Ljava/lang/String;)V",
                       GetJNIBrowser(browser), NewJNIString(env, value));
}

bool DisplayHandler::OnConsoleMessage(CefRefPtr<CefBrowser> browser,
                                      cef_log_severity_t level,
                                      const CefString& message,
                                      const CefString& source,
                                      int line) {
  JNIEnv* env = GetJNIEnv();
  if (!env)
    return false;

  jobject jlevel = NULL;
  switch (level) {
    JNI_CASE(env, "org/cef/CefSettings$LogSeverity", LOGSEVERITY_VERBOSE,
             jlevel);
    JNI_CASE(env, "org/cef/CefSettings$LogSeverity", LOGSEVERITY_INFO, jlevel);
    JNI_CASE(env, "org/cef/CefSettings$LogSeverity", LOGSEVERITY_WARNING,
             jlevel);
    JNI_CASE(env, "org/cef/CefSettings$LogSeverity", LOGSEVERITY_ERROR, jlevel);
    JNI_CASE(env, "org/cef/CefSettings$LogSeverity", LOGSEVERITY_DISABLE,
             jlevel);
    JNI_CASE(env, "org/cef/CefSettings$LogSeverity", LOGSEVERITY_DEFAULT,
             jlevel);
  }

  jboolean jreturn = JNI_FALSE;
  JNI_CALL_METHOD(
      env, jhandler_, "onConsoleMessage",
      "(Lorg/cef/browser/CefBrowser;Lorg/cef/CefSettings$LogSeverity;"
      "Ljava/lang/String;Ljava/lang/String;I)Z",
      Boolean, jreturn, GetJNIBrowser(browser), jlevel,
      NewJNIString(env, message), NewJNIString(env, source), line);
  return (jreturn != JNI_FALSE);
}
