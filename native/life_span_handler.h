// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef JCEF_NATIVE_LIFE_SPAN_HANDLER_H_
#define JCEF_NATIVE_LIFE_SPAN_HANDLER_H_
#pragma once

#include <list>
#include <jni.h>
#include "include/cef_life_span_handler.h"

// LifeSpanHandler implementation.
class LifeSpanHandler : public CefLifeSpanHandler {
 public:
  LifeSpanHandler(JNIEnv* env, jobject handler);
  virtual ~LifeSpanHandler();

  // CefLifeSpanHandler methods
  virtual bool OnBeforePopup(CefRefPtr<CefBrowser> browser,
                             CefRefPtr<CefFrame> frame,
                             const CefString& target_url,
                             const CefString& target_frame_name,
                             WindowOpenDisposition target_disposition,
                             bool user_gesture,
                             const CefPopupFeatures& popupFeatures,
                             CefWindowInfo& windowInfo,
                             CefRefPtr<CefClient>& client,
                             CefBrowserSettings& settings,
                             bool* no_javascript_access) OVERRIDE;
  virtual void OnAfterCreated(CefRefPtr<CefBrowser> browser) OVERRIDE;
  virtual bool DoClose(CefRefPtr<CefBrowser> browser) OVERRIDE;
  virtual void OnBeforeClose(CefRefPtr<CefBrowser> browser) OVERRIDE;

  void registerJBrowser(jobject browser);
  void unregisterJBrowser(jobject browser);
 protected:
  jobject jhandler_;
  std::list<jobject> jbrowsers_;

  // Include the default reference counting implementation.
  IMPLEMENT_REFCOUNTING(LifeSpanHandler);
};

#endif  // JCEF_NATIVE_LIFE_SPAN_HANDLER_H_
