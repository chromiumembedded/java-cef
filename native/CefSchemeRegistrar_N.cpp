// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "CefSchemeRegistrar_N.h"

#include <fstream>

#include "client_app.h"
#include "jni_util.h"
#include "util.h"
#include "include/cef_scheme.h"

JNIEXPORT jboolean JNICALL Java_org_cef_callback_CefSchemeRegistrar_1N_N_1AddCustomScheme
  (JNIEnv *env, jobject obj, jstring jSchemeName, jboolean jIsStandard, 
   jboolean jIsLocal, jboolean jIsDisplayIsolated, jboolean jIsSecure,
   jboolean jIsCorsEnabled) {

  CefRawPtr<CefSchemeRegistrar> registrar =
      GetCefFromJNIObject<CefSchemeRegistrar>(env, obj, "CefSchemeRegistrar");
  if (!registrar)
    return JNI_FALSE;
  CefString schemeName = GetJNIString(env, jSchemeName);
  bool result = registrar->AddCustomScheme(schemeName,
                                           jIsStandard != JNI_FALSE,
                                           jIsLocal != JNI_FALSE,
                                           jIsDisplayIsolated != JNI_FALSE,
                                           jIsSecure != JNI_FALSE,
                                           jIsCorsEnabled != JNI_FALSE);
  if (!result)
    return JNI_FALSE;

  // The registered scheme has to be forwarded to all other processes which will
  // be created by the browser process (e.g. the render-process). Otherwise
  // things like JS "localStorage" get/set will end up in a crashed
  // render process.
  std::string tmpName = util::GetTempFileName("scheme", false);

  std::ofstream fStream(tmpName.c_str(), std::ofstream::out |
      std::ofstream::app);

  if (fStream.is_open()) {
    // 1) Write the scheme name and the params to the file.
    fStream << schemeName.ToString().c_str() << "," <<
        (jIsStandard != JNI_FALSE) << (jIsLocal != JNI_FALSE) <<
        (jIsDisplayIsolated != JNI_FALSE) << (jIsSecure != JNI_FALSE) <<
        (jIsCorsEnabled != JNI_FALSE);
    fStream.close();

    // 2) Register file to be deleted in CefShutdown()
    ClientApp::registerTempFile(tmpName);
  }
  return JNI_TRUE;
}
