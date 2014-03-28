// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "life_span_handler.h"
#include "client_handler.h"

#include "jni_util.h"
#include "util.h"

LifeSpanHandler::LifeSpanHandler(JNIEnv* env, jobject handler) {
  jhandler_ = env->NewGlobalRef(handler);
}

LifeSpanHandler::~LifeSpanHandler() {
  JNIEnv* env = GetJNIEnv();
  env->DeleteGlobalRef(jhandler_);
}

bool LifeSpanHandler::OnBeforePopup(CefRefPtr<CefBrowser> browser,
                                    CefRefPtr<CefFrame> frame,
                                    const CefString& target_url,
                                    const CefString& target_frame_name,
                                    const CefPopupFeatures& popupFeatures,
                                    CefWindowInfo& windowInfo,
                                    CefRefPtr<CefClient>& client,
                                    CefBrowserSettings& settings,
                                    bool* no_javascript_access) {
  if (browser->GetHost()->IsWindowRenderingDisabled()) {
    // Cancel popups in off-screen rendering mode.
    return true;
  }
  JNIEnv* env = GetJNIEnv();
  if (!env)
    return false;
  jboolean jreturn = JNI_FALSE;
  JNI_CALL_METHOD(env, jhandler_,
                  "onBeforePopup",
                  "(Lorg/cef/CefBrowser;Ljava/lang/String;Ljava/lang/String;)Z",
                  Boolean,
                  jreturn,
                  GetJNIBrowser(browser),
                  NewJNIString(env, target_url),
                  NewJNIString(env, target_frame_name));
  return (jreturn == JNI_TRUE);
}

void LifeSpanHandler::OnAfterCreated(CefRefPtr<CefBrowser> browser) {
  CefRefPtr<ClientHandler> client = (ClientHandler*)browser->GetHost()->GetClient().get();
  client->SetBrowser(browser);

  JNIEnv* env = GetJNIEnv();
  if (!env)
  	return;
  JNI_CALL_VOID_METHOD(env, jhandler_, 
                       "onAfterCreated", 
                      "(Lorg/cef/CefBrowser;)V",
                       GetJNIBrowser(browser));
}

bool LifeSpanHandler::RunModal(CefRefPtr<CefBrowser> browser) {
  JNIEnv* env = GetJNIEnv();
  if (!env)
    return false;
  jboolean jreturn = JNI_FALSE;
  JNI_CALL_METHOD(env, jhandler_,
                  "runModal",
                  "(Lorg/cef/CefBrowser;)Z",
                  Boolean,
                  jreturn,
                  GetJNIBrowser(browser));
  return (jreturn == JNI_TRUE);
}

bool LifeSpanHandler::DoClose(CefRefPtr<CefBrowser> browser) {
  JNIEnv* env = GetJNIEnv();
  if (!env)
    return false;
  jboolean jreturn = JNI_FALSE;
  JNI_CALL_METHOD(env, jhandler_,
                  "doClose",
                  "(Lorg/cef/CefBrowser;)Z",
                  Boolean,
                  jreturn,
                  GetJNIBrowser(browser));
  return (jreturn == JNI_TRUE);
}

void LifeSpanHandler::OnBeforeClose(CefRefPtr<CefBrowser> browser) {
  REQUIRE_UI_THREAD();
  JNIEnv* env = GetJNIEnv();
  if (!env)
    return;
  JNI_CALL_VOID_METHOD(env, jhandler_,
                       "onBeforeClose",
                       "(Lorg/cef/CefBrowser;)V",
                       GetJNIBrowser(browser));
  CefRefPtr<ClientHandler> client = (ClientHandler*)browser->GetHost()->GetClient().get();
  client->RemoveBrowser(browser);
}
