// Copyright (c) 2017 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "pdf_print_callback.h"

#include "jni_scoped_helpers.h"
#include "jni_util.h"
#include "util.h"

PdfPrintCallback::PdfPrintCallback(JNIEnv* env, jobject jcallback)
    : handle_(env, jcallback) {}

void PdfPrintCallback::OnPdfPrintFinished(const CefString& path, bool ok) {
  ScopedJNIEnv env;
  if (!env)
    return;

  ScopedJNIString jpath(env, path);
  JNI_CALL_VOID_METHOD(env, handle_, "onPdfPrintFinished",
                       "(Ljava/lang/String;Z)V", jpath.get(), (jboolean)ok);
}
