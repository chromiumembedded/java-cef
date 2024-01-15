// Copyright (c) 2024 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "CefRegistration_N.h"
#include "include/cef_registration.h"
#include "jni_scoped_helpers.h"

JNIEXPORT void JNICALL
Java_org_cef_browser_CefRegistration_1N_N_1Dispose(JNIEnv* env,
                                                   jobject obj,
                                                   jlong self) {
  SetCefForJNIObject<CefRegistration>(env, obj, NULL, "CefRegistration");
}
