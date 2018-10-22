// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "dialog_handler.h"

#include "client_handler.h"
#include "jni_util.h"
#include "util.h"

DialogHandler::DialogHandler(JNIEnv* env, jobject handler) {
  jhandler_ = env->NewGlobalRef(handler);
}

DialogHandler::~DialogHandler() {
  JNIEnv* env = GetJNIEnv();
  env->DeleteGlobalRef(jhandler_);
}

bool DialogHandler::OnFileDialog(CefRefPtr<CefBrowser> browser,
                                 FileDialogMode mode,
                                 const CefString& title,
                                 const CefString& default_file_path,
                                 const std::vector<CefString>& accept_filters,
                                 int selected_accept_filter,
                                 CefRefPtr<CefFileDialogCallback> callback) {
  JNIEnv* env = GetJNIEnv();
  if (!env)
    return false;

  jobject jmode = NULL;
  switch (mode) {
    default:
      JNI_CASE(env, "org/cef/handler/CefDialogHandler$FileDialogMode",
               FILE_DIALOG_OPEN, jmode);
      JNI_CASE(env, "org/cef/handler/CefDialogHandler$FileDialogMode",
               FILE_DIALOG_OPEN_MULTIPLE, jmode);
      JNI_CASE(env, "org/cef/handler/CefDialogHandler$FileDialogMode",
               FILE_DIALOG_SAVE, jmode);
  }

  jobject jcallback =
      NewJNIObject(env, "org/cef/callback/CefFileDialogCallback_N");
  if (!jcallback)
    return false;
  SetCefForJNIObject(env, jcallback, callback.get(), "CefFileDialogCallback");

  jboolean jreturn = JNI_FALSE;
  jstring jtitle = NewJNIString(env, title);
  jstring jdefault_file_path = NewJNIString(env, default_file_path);
  jobject jaccept_filters = NewJNIStringVector(env, accept_filters);
  JNI_CALL_METHOD(
      env, jhandler_, "onFileDialog",
      "(Lorg/cef/browser/CefBrowser;Lorg/cef/handler/"
      "CefDialogHandler$FileDialogMode;Ljava/lang/String;Ljava/lang/"
      "String;Ljava/util/Vector;ILorg/cef/callback/CefFileDialogCallback;)Z",
      Boolean, jreturn, GetJNIBrowser(browser), jmode, jtitle,
      jdefault_file_path, jaccept_filters, selected_accept_filter, jcallback);
  env->DeleteLocalRef(jaccept_filters);
  env->DeleteLocalRef(jdefault_file_path);
  env->DeleteLocalRef(jtitle);
  env->DeleteLocalRef(jcallback);
  return (jreturn != JNI_FALSE);
}
