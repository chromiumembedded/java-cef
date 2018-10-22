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

// TODO(JCEF): Expose all parameters.
bool LifeSpanHandler::OnBeforePopup(CefRefPtr<CefBrowser> browser,
                                    CefRefPtr<CefFrame> frame,
                                    const CefString& target_url,
                                    const CefString& target_frame_name,
                                    WindowOpenDisposition target_disposition,
                                    bool user_gesture,
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
  jobject jframe = GetJNIFrame(env, frame);
  jstring jtarget_url = NewJNIString(env, target_url);
  jstring jtarget_frame_name = NewJNIString(env, target_frame_name);
  JNI_CALL_METHOD(env, jhandler_, "onBeforePopup",
                  "(Lorg/cef/browser/CefBrowser;Lorg/cef/browser/"
                  "CefFrame;Ljava/lang/String;Ljava/lang/String;)Z",
                  Boolean, jreturn, GetJNIBrowser(browser), jframe, jtarget_url,
                  jtarget_frame_name);
  if (jframe)
    env->DeleteLocalRef(jframe);
  env->DeleteLocalRef(jtarget_url);
  env->DeleteLocalRef(jtarget_frame_name);
  return (jreturn != JNI_FALSE);
}

void LifeSpanHandler::OnAfterCreated(CefRefPtr<CefBrowser> browser) {
  JNIEnv* env = GetJNIEnv();
  if (!env || jbrowsers_.empty())
    return;

  util::AddCefBrowser(browser);

  jobject jbrowser = jbrowsers_.front();
  jbrowsers_.pop_front();

  CefRefPtr<ClientHandler> client =
      (ClientHandler*)browser->GetHost()->GetClient().get();
  client->OnAfterCreated();

  if (SetCefForJNIObject(env, jbrowser, browser.get(), "CefBrowser")) {
    JNI_CALL_VOID_METHOD(env, jhandler_, "onAfterCreated",
                         "(Lorg/cef/browser/CefBrowser;)V", jbrowser);
  }
  env->DeleteGlobalRef(jbrowser);
}

bool LifeSpanHandler::DoClose(CefRefPtr<CefBrowser> browser) {
  JNIEnv* env = GetJNIEnv();
  if (!env)
    return false;
  jboolean jreturn = JNI_FALSE;
  JNI_CALL_METHOD(env, jhandler_, "doClose", "(Lorg/cef/browser/CefBrowser;)Z",
                  Boolean, jreturn, GetJNIBrowser(browser));
  return (jreturn != JNI_FALSE);
}

void LifeSpanHandler::OnBeforeClose(CefRefPtr<CefBrowser> browser) {
  REQUIRE_UI_THREAD();
  JNIEnv* env = GetJNIEnv();
  if (!env)
    return;
  jobject jbrowser = GetJNIBrowser(browser);
  JNI_CALL_VOID_METHOD(env, jhandler_, "onBeforeClose",
                       "(Lorg/cef/browser/CefBrowser;)V", jbrowser);

  // Clear the browser pointer member of the Java object. This call will
  // release the extra reference to the object added in
  // LifeSpanHandler::OnAfterCreated.
  SetCefForJNIObject<CefBrowser>(env, jbrowser, NULL, "CefBrowser");

  CefRefPtr<ClientHandler> client =
      (ClientHandler*)browser->GetHost()->GetClient().get();
  client->OnBeforeClose(browser);
}

void LifeSpanHandler::registerJBrowser(jobject browser) {
  jbrowsers_.push_back(browser);
}

void LifeSpanHandler::unregisterJBrowser(jobject browser) {
  jbrowsers_.remove(browser);
}
