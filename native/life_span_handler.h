// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef JCEF_NATIVE_LIFE_SPAN_HANDLER_H_
#define JCEF_NATIVE_LIFE_SPAN_HANDLER_H_
#pragma once

#include <jni.h>

#include <list>

#include "include/cef_life_span_handler.h"

#include "jni_scoped_helpers.h"

// LifeSpanHandler implementation.
class LifeSpanHandler : public CefLifeSpanHandler {
 public:
  LifeSpanHandler(JNIEnv* env, jobject handler);

  // CefLifeSpanHandler methods:
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
                             CefRefPtr<CefDictionaryValue>& extra_info,
                             bool* no_javascript_access) override;
  virtual void OnAfterCreated(CefRefPtr<CefBrowser> browser) override;
  virtual bool DoClose(CefRefPtr<CefBrowser> browser) override;
  virtual void OnBeforeClose(CefRefPtr<CefBrowser> browser) override;

  void OnAfterParentChanged(CefRefPtr<CefBrowser> browser);

  void registerJBrowser(jobject browser);
  void unregisterJBrowser(jobject browser);

 protected:
  ScopedJNIObjectGlobal handle_;
  std::list<jobject> jbrowsers_;

  // Include the default reference counting implementation.
  IMPLEMENT_REFCOUNTING(LifeSpanHandler);
};

#endif  // JCEF_NATIVE_LIFE_SPAN_HANDLER_H_
