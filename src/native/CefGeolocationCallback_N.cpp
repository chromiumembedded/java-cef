// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "CefGeolocationCallback_N.h"
#include "include/cef_geolocation_handler.h"
#include "jni_util.h"

JNIEXPORT void JNICALL Java_org_cef_callback_CefGeolocationCallback_1N_N_1Continue
  (JNIEnv *env, jobject obj, jboolean jallow) {
    CefRefPtr<CefGeolocationCallback> geolocationCallback =
      GetCefFromJNIObject<CefGeolocationCallback>(env, obj, "CefGeolocationCallback");
  if (!geolocationCallback.get())
    return;
  geolocationCallback->Continue(jallow != JNI_FALSE);

  // Clear the reference added in GeolocationHandler::OnRequestGeolocationPermission.
  SetCefForJNIObject<CefGeolocationCallback>(env, obj, NULL, "CefGeolocationCallback");
}
