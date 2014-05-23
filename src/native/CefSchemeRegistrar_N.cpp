// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "CefSchemeRegistrar_N.h"
#include "jni_util.h"
#include "include/cef_scheme.h"

JNIEXPORT jboolean JNICALL Java_org_cef_callback_CefSchemeRegistrar_1N_N_1AddCustomScheme
  (JNIEnv *env, jobject obj, jstring jSchemeName, jboolean jIsStandard, 
   jboolean jIsLocal, jboolean jIsDisplayIsolated) {

  CefRefPtr<CefSchemeRegistrar> registrar =
      GetCefFromJNIObject<CefSchemeRegistrar>(env, obj, "CefSchemeRegistrar");
  if (!registrar.get())
    return JNI_FALSE;
  bool result = registrar->AddCustomScheme(GetJNIString(env, jSchemeName),
                                           jIsStandard != JNI_FALSE,
                                           jIsLocal != JNI_FALSE,
                                           jIsDisplayIsolated != JNI_FALSE);
  return result ? JNI_TRUE : JNI_FALSE;
}
