// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef CEF_TESTS_CEFCLIENT_GEOLOCATION_HANDLER_H_
#define CEF_TESTS_CEFCLIENT_GEOLOCATION_HANDLER_H_
#pragma once

#include <jni.h>
#include "include/cef_geolocation_handler.h"

// GeolocationHandler implementation.
class GeolocationHandler : public CefGeolocationHandler {
 public:
  GeolocationHandler(JNIEnv* env, jobject handler);
  virtual ~GeolocationHandler();

  // CefGeolocationHandler methods
  virtual void OnRequestGeolocationPermission(
      CefRefPtr<CefBrowser> browser,
      const CefString& requesting_url,
      int request_id,
      CefRefPtr<CefGeolocationCallback> callback) OVERRIDE;

  virtual void OnCancelGeolocationPermission(
      CefRefPtr<CefBrowser> browser,
      const CefString& requesting_url,
      int request_id) OVERRIDE;

 protected:
  jobject jhandler_;

  // Include the default reference counting implementation.
  IMPLEMENT_REFCOUNTING(GeolocationHandler);
  // Include the default locking implementation.
  IMPLEMENT_LOCKING(GeolocationHandler);
};

#endif  // CEF_TESTS_CEFCLIENT_GEOLOCATION_HANDLER_H_
