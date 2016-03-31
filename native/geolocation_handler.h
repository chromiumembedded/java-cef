// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef JCEF_NATIVE_GEOLOCATION_HANDLER_H_
#define JCEF_NATIVE_GEOLOCATION_HANDLER_H_
#pragma once

#include <jni.h>
#include "include/cef_geolocation_handler.h"

// GeolocationHandler implementation.
class GeolocationHandler : public CefGeolocationHandler {
 public:
  GeolocationHandler(JNIEnv* env, jobject handler);
  virtual ~GeolocationHandler();

  // CefGeolocationHandler methods
  virtual bool OnRequestGeolocationPermission(
      CefRefPtr<CefBrowser> browser,
      const CefString& requesting_url,
      int request_id,
      CefRefPtr<CefGeolocationCallback> callback) OVERRIDE;

  virtual void OnCancelGeolocationPermission(
      CefRefPtr<CefBrowser> browser,
      int request_id) OVERRIDE;

 protected:
  jobject jhandler_;

  // Include the default reference counting implementation.
  IMPLEMENT_REFCOUNTING(GeolocationHandler);
};

#endif  // JCEF_NATIVE_GEOLOCATION_HANDLER_H_
