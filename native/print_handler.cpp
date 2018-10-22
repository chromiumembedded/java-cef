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

  JNI_CALL_VOID_METHOD(env, jhandler_, "onPrintStart",
                       "(Lorg/cef/browser/CefBrowser;)V",
                       GetJNIBrowser(browser));
}

void PrintHandler::OnPrintSettings(CefRefPtr<CefBrowser> browser,
                                   CefRefPtr<CefPrintSettings> settings,
                                   bool get_defaults) {
  JNIEnv* env = GetJNIEnv();
  if (!env)
    return;

  jobject jsettings = NewJNIObject(env, "org/cef/misc/CefPrintSettings_N");
  if (!jsettings)
    return;
  SetCefForJNIObject(env, jsettings, settings.get(), "CefPrintSettings");

  JNI_CALL_VOID_METHOD(env, jhandler_, "onPrintSettings",
                       "(Lorg/cef/misc/CefPrintSettings;Z)V", jsettings,
                       get_defaults ? JNI_TRUE : JNI_FALSE);

  // Do not keep a reference to |settings| outside of this callback.
  SetCefForJNIObject<CefPrintSettings>(env, jsettings, NULL,
                                       "CefPrintSettings");
  env->DeleteLocalRef(jsettings);
}

bool PrintHandler::OnPrintDialog(CefRefPtr<CefBrowser> browser,
                                 bool has_selection,
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
  JNI_CALL_METHOD(env, jhandler_, "onPrintDialog",
                  "(ZLorg/cef/callback/CefPrintDialogCallback;)Z", Boolean,
                  jresult, (has_selection ? JNI_TRUE : JNI_FALSE), jcallback);

  if (jresult == JNI_FALSE) {
    // delete CefPrintDialogCallback reference from Java
    SetCefForJNIObject<CefPrintDialogCallback>(env, jcallback, NULL,
                                               "CefPrintDialogCallback");
  }
  env->DeleteLocalRef(jcallback);
  return (jresult != JNI_FALSE);
}

bool PrintHandler::OnPrintJob(CefRefPtr<CefBrowser> browser,
                              const CefString& document_name,
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
  jstring jdocument_name = NewJNIString(env, document_name);
  jstring jpdf_file_path = NewJNIString(env, pdf_file_path);
  JNI_CALL_METHOD(env, jhandler_, "onPrintJob",
                  "(Ljava/lang/String;Ljava/lang/String;"
                  "Lorg/cef/callback/CefPrintJobCallback;)Z",
                  Boolean, jresult, jdocument_name, jpdf_file_path, jcallback);

  if (jresult == JNI_FALSE) {
    // delete CefPrintDialogCallback reference from Java
    SetCefForJNIObject<CefPrintDialogCallback>(env, jcallback, NULL,
                                               "CefPrintJobCallback");
  }
  env->DeleteLocalRef(jpdf_file_path);
  env->DeleteLocalRef(jdocument_name);
  env->DeleteLocalRef(jcallback);
  return (jresult != JNI_FALSE);
}

void PrintHandler::OnPrintReset(CefRefPtr<CefBrowser> browser) {
  JNIEnv* env = GetJNIEnv();
  if (!env)
    return;
  JNI_CALL_VOID_METHOD(env, jhandler_, "onPrintReset", "()V");
}

CefSize PrintHandler::GetPdfPaperSize(int device_units_per_inch) {
  JNIEnv* env = GetJNIEnv();
  if (!env)
    return CefSize(0, 0);

  jobject jsize = NewJNIObject(env, "java/awt/Dimension");
  if (!jsize)
    return CefSize(0, 0);

  JNI_CALL_METHOD(env, jhandler_, "getPdfPaperSize", "(I)Ljava/awt/Dimension;",
                  Object, jsize, (jint)device_units_per_inch);

  CefSize size = GetJNISize(env, jsize);

  env->DeleteLocalRef(jsize);

  return size;
}
