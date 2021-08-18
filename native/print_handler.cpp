// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "print_handler.h"

#include "jni_util.h"
#include "util.h"

namespace {

// JNI CefPrintDialogCallback object.
class ScopedJNIPrintDialogCallback
    : public ScopedJNIObject<CefPrintDialogCallback> {
 public:
  ScopedJNIPrintDialogCallback(JNIEnv* env,
                               CefRefPtr<CefPrintDialogCallback> obj)
      : ScopedJNIObject<CefPrintDialogCallback>(
            env,
            obj,
            "org/cef/callback/CefPrintDialogCallback_N",
            "CefPrintDialogCallback") {}
};

// JNI CefPrintJobCallback object.
class ScopedJNIPrintJobCallback : public ScopedJNIObject<CefPrintJobCallback> {
 public:
  ScopedJNIPrintJobCallback(JNIEnv* env, CefRefPtr<CefPrintJobCallback> obj)
      : ScopedJNIObject<CefPrintJobCallback>(
            env,
            obj,
            "org/cef/callback/CefPrintJobCallback_N",
            "CefPrintJobCallback") {}
};

}  // namespace

PrintHandler::PrintHandler(JNIEnv* env, jobject handler)
    : handle_(env, handler) {}

void PrintHandler::OnPrintStart(CefRefPtr<CefBrowser> browser) {
  ScopedJNIEnv env;
  if (!env)
    return;

  ScopedJNIBrowser jbrowser(env, browser);

  JNI_CALL_VOID_METHOD(env, handle_, "onPrintStart",
                       "(Lorg/cef/browser/CefBrowser;)V", jbrowser.get());
}

void PrintHandler::OnPrintSettings(CefRefPtr<CefBrowser> browser,
                                   CefRefPtr<CefPrintSettings> settings,
                                   bool get_defaults) {
  ScopedJNIEnv env;
  if (!env)
    return;

  ScopedJNIBrowser jbrowser(env, browser);
  ScopedJNIPrintSettings jsettings(env, settings);
  jsettings.SetTemporary();

  JNI_CALL_VOID_METHOD(
      env, handle_, "onPrintSettings",
      "(Lorg/cef/browser/CefBrowser;Lorg/cef/misc/CefPrintSettings;Z)V",
      jbrowser.get(), jsettings.get(), get_defaults ? JNI_TRUE : JNI_FALSE);
}

bool PrintHandler::OnPrintDialog(CefRefPtr<CefBrowser> browser,
                                 bool has_selection,
                                 CefRefPtr<CefPrintDialogCallback> callback) {
  ScopedJNIEnv env;
  if (!env)
    return false;

  ScopedJNIBrowser jbrowser(env, browser);
  ScopedJNIPrintDialogCallback jcallback(env, callback);

  jboolean jresult = JNI_FALSE;

  JNI_CALL_METHOD(env, handle_, "onPrintDialog",
                  "(Lorg/cef/browser/CefBrowser;ZLorg/cef/callback/"
                  "CefPrintDialogCallback;)Z",
                  Boolean, jresult, jbrowser.get(),
                  (has_selection ? JNI_TRUE : JNI_FALSE), jcallback.get());

  if (jresult == JNI_FALSE) {
    // If the Java method returns "false" the callback won't be used and
    // the reference can therefore be removed.
    jcallback.SetTemporary();
  }

  return (jresult != JNI_FALSE);
}

bool PrintHandler::OnPrintJob(CefRefPtr<CefBrowser> browser,
                              const CefString& document_name,
                              const CefString& pdf_file_path,
                              CefRefPtr<CefPrintJobCallback> callback) {
  ScopedJNIEnv env;
  if (!env)
    return false;

  ScopedJNIBrowser jbrowser(env, browser);
  ScopedJNIString jdocumentName(env, document_name);
  ScopedJNIString jpdfFilePath(env, pdf_file_path);
  ScopedJNIPrintJobCallback jcallback(env, callback);

  jboolean jresult = JNI_FALSE;

  JNI_CALL_METHOD(
      env, handle_, "onPrintJob",
      "(Lorg/cef/browser/CefBrowser;Ljava/lang/String;Ljava/lang/String;"
      "Lorg/cef/callback/CefPrintJobCallback;)Z",
      Boolean, jresult, jbrowser.get(), jdocumentName.get(), jpdfFilePath.get(),
      jcallback.get());

  if (jresult == JNI_FALSE) {
    // If the Java method returns "false" the callback won't be used and
    // the reference can therefore be removed.
    jcallback.SetTemporary();
  }

  return (jresult != JNI_FALSE);
}

void PrintHandler::OnPrintReset(CefRefPtr<CefBrowser> browser) {
  ScopedJNIEnv env;
  if (!env)
    return;

  ScopedJNIBrowser jbrowser(env, browser);

  JNI_CALL_VOID_METHOD(env, handle_, "onPrintReset",
                       "(Lorg/cef/browser/CefBrowser;)V", jbrowser.get());
}

CefSize PrintHandler::GetPdfPaperSize(CefRefPtr<CefBrowser> browser,
                                      int device_units_per_inch) {
  ScopedJNIEnv env;
  if (!env)
    return CefSize(0, 0);

  ScopedJNIBrowser jbrowser(env, browser);
  ScopedJNIObjectResult jresult(env);

  JNI_CALL_METHOD(env, handle_, "getPdfPaperSize",
                  "(Lorg/cef/browser/CefBrowser;I)Ljava/awt/Dimension;", Object,
                  jresult, jbrowser.get(), (jint)device_units_per_inch);
  if (!jresult)
    return CefSize(0, 0);

  return GetJNISize(env, jresult);
}
