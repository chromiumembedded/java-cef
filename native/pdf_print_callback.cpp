// Copyright (c) 2017 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "pdf_print_callback.h"

#include "jni_util.h"
#include "util.h"

PdfPrintCallback::PdfPrintCallback(JNIEnv* env, jobject jcallback) {
  jcallback_ = env->NewGlobalRef(jcallback);
}

PdfPrintCallback::~PdfPrintCallback() {
  JNIEnv* env = GetJNIEnv();
  env->DeleteGlobalRef(jcallback_);
}

void PdfPrintCallback::OnPdfPrintFinished(const CefString& path, bool ok) {
  JNIEnv* env = GetJNIEnv();
  if (!env)
    return;

  jstring jpath = NewJNIString(env, path);
  JNI_CALL_VOID_METHOD(env, jcallback_, "onPdfPrintFinished",
                       "(Ljava/lang/String;Z)V", jpath, (jboolean)ok);
  env->DeleteLocalRef(jpath);
}
