// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "focus_handler.h"

#include "include/base/cef_bind.h"
#include "include/wrapper/cef_closure_task.h"

#include "client_handler.h"
#include "jni_util.h"
#include "util.h"

FocusHandler::FocusHandler(JNIEnv* env, jobject handler) {
  jhandler_ = env->NewGlobalRef(handler);
}

FocusHandler::~FocusHandler() {
  JNIEnv* env = GetJNIEnv();
  env->DeleteGlobalRef(jhandler_);
}

void FocusHandler::OnTakeFocus(CefRefPtr<CefBrowser> browser, bool next) {
  JNIEnv* env = GetJNIEnv();
  if (!env)
    return;
  JNI_CALL_VOID_METHOD(env, jhandler_, "onTakeFocus",
                       "(Lorg/cef/browser/CefBrowser;Z)V",
                       GetJNIBrowser(browser), (jboolean)next);
}

bool FocusHandler::OnSetFocus(CefRefPtr<CefBrowser> browser,
                              FocusSource source) {
  JNIEnv* env = GetJNIEnv();
  if (!env)
    return false;
  jboolean jreturn = JNI_FALSE;
  jobject jsource = NULL;
  switch (source) {
    JNI_CASE(env, "org/cef/handler/CefFocusHandler$FocusSource",
             FOCUS_SOURCE_NAVIGATION, jsource);
    default:
      JNI_CASE(env, "org/cef/handler/CefFocusHandler$FocusSource",
               FOCUS_SOURCE_SYSTEM, jsource);
  }

  JNI_CALL_METHOD(env, jhandler_, "onSetFocus",
                  "(Lorg/cef/browser/CefBrowser;Lorg/cef/handler/"
                  "CefFocusHandler$FocusSource;)Z",
                  Boolean, jreturn, GetJNIBrowser(browser), jsource);
  bool result = (jreturn != JNI_FALSE);
#if (defined(OS_WIN) || defined(OS_LINUX))
  if (result) {
    CefWindowHandle browserHandle = browser->GetHost()->GetWindowHandle();
    if (CefCurrentlyOn(TID_UI))
      util::FocusParent(browserHandle);
    else
      CefPostTask(TID_UI, base::Bind(util::FocusParent, browserHandle));
  }
#endif
  return result;
}

void FocusHandler::OnGotFocus(CefRefPtr<CefBrowser> browser) {
  JNIEnv* env = GetJNIEnv();
  if (!env)
    return;
  JNI_CALL_VOID_METHOD(env, jhandler_, "onGotFocus",
                       "(Lorg/cef/browser/CefBrowser;)V",
                       GetJNIBrowser(browser));
}
