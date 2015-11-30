// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "print_handler.h"

#include "jni_util.h"
#include "util.h"

PrintHandler::PrintHandler(JNIEnv* env, jobject handler) {
  jhandler_ = env->NewGlobalRef(handler);
}

PrintHandler::~PrintHandler() {
  JNIEnv* env = GetJNIEnv();
  env->DeleteGlobalRef(jhandler_);
}

void PrintHandler::OnPrintStart(CefRefPtr<CefBrowser> browser) {
  JNIEnv* env = GetJNIEnv();
  if (!env)
    return;

  JNI_CALL_VOID_METHOD(env, jhandler_,
                       "onPrintStart",
                       "(Lorg/cef/browser/CefBrowser;)V",
                       GetJNIBrowser(browser));
}

void PrintHandler::OnPrintSettings(CefRefPtr<CefPrintSettings> settings,
                                   bool get_defaults) {
  JNIEnv* env = GetJNIEnv();
  if (!env)
    return;

  jobject jsettings = NewJNIObject(env, "org/cef/misc/CefPrintSettings_N");
  if (!jsettings)
    return;
  SetCefForJNIObject(env, jsettings, settings.get(), "CefPrintSettings");

  JNI_CALL_VOID_METHOD(env, jhandler_,
                       "onPrintSettings",
                       "(Lorg/cef/misc/CefPrintSettings;Z)V",
                       jsettings,
                       get_defaults ? JNI_TRUE : JNI_FALSE);

  // Do not keep a reference to |settings| outside of this callback.
  SetCefForJNIObject<CefPrintSettings>(env, jsettings, NULL,
      "CefPrintSettings");
}

bool PrintHandler::OnPrintDialog(bool has_selection,
                                 CefRefPtr<CefPrintDialogCallback> callback) {
  JNIEnv* env = GetJNIEnv();
  if (!env)
    return false;

  jobject jcallback =
      NewJNIObject(env, "org/cef/callback/CefPrintDialogCallback_N");
  if (!jcallback)
    return false;
  SetCefForJNIObject(env, jcallback, callback.get(), "CefPrintDialogCallback");

  jboolean jresult = JNI_FALSE;
  JNI_CALL_METHOD(env, jhandler_,
                  "onPrintDialog",
                  "(ZLorg/cef/callback/CefPrintDialogCallback;)Z",
                  Boolean,
                  jresult,
                  (has_selection ? JNI_TRUE : JNI_FALSE),
                  jcallback);

  if (jresult == JNI_FALSE) {
    // delete CefPrintDialogCallback reference from Java
    SetCefForJNIObject<CefPrintDialogCallback>(env,
                                              jcallback,
                                              NULL,
                                              "CefPrintDialogCallback");
  }
  return (jresult != JNI_FALSE);
}

bool PrintHandler::OnPrintJob(const CefString& document_name,
                              const CefString& pdf_file_path,
                              CefRefPtr<CefPrintJobCallback> callback) {
  JNIEnv* env = GetJNIEnv();
  if (!env)
    return false;

  jobject jcallback =
      NewJNIObject(env, "org/cef/callback/CefPrintJobCallback_N");
  if (!jcallback)
    return false;
  SetCefForJNIObject(env, jcallback, callback.get(), "CefPrintJobCallback");

  jboolean jresult = JNI_FALSE;
  JNI_CALL_METHOD(env, jhandler_,
                  "onPrintJob",
                  "(Ljava/lang/String;Ljava/lang/String;"
                  "Lorg/cef/callback/CefPrintJobCallback;)Z",
                  Boolean,
                  jresult,
                  NewJNIString(env, document_name),
                  NewJNIString(env, pdf_file_path),
                  jcallback);

  if (jresult == JNI_FALSE) {
    // delete CefPrintDialogCallback reference from Java
    SetCefForJNIObject<CefPrintDialogCallback>(env,
                                              jcallback,
                                              NULL,
                                              "CefPrintJobCallback");
  }
  return (jresult != JNI_FALSE);
}

void PrintHandler::OnPrintReset() {
  JNIEnv* env = GetJNIEnv();
  if (!env)
    return;
  JNI_CALL_VOID_METHOD(env, jhandler_, "onPrintReset", "()V");
}
