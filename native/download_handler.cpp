// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "download_handler.h"

#include "jni_util.h"

namespace {

// JNI CefDownloadItem object.
class ScopedJNIDownloadItem : public ScopedJNIObject<CefDownloadItem> {
 public:
  ScopedJNIDownloadItem(JNIEnv* env, CefRefPtr<CefDownloadItem> obj)
      : ScopedJNIObject<CefDownloadItem>(env,
                                         obj,
                                         "org/cef/callback/CefDownloadItem_N",
                                         "CefDownloadItem") {}
};

// JNI CefBeforeDownloadCallback object.
class ScopedJNIBeforeDownloadCallback
    : public ScopedJNIObject<CefBeforeDownloadCallback> {
 public:
  ScopedJNIBeforeDownloadCallback(JNIEnv* env,
                                  CefRefPtr<CefBeforeDownloadCallback> obj)
      : ScopedJNIObject<CefBeforeDownloadCallback>(
            env,
            obj,
            "org/cef/callback/CefBeforeDownloadCallback_N",
            "CefBeforeDownloadCallback") {}
};

// JNI CefDownloadItemCallback object.
class ScopedJNIDownloadItemCallback
    : public ScopedJNIObject<CefDownloadItemCallback> {
 public:
  ScopedJNIDownloadItemCallback(JNIEnv* env,
                                CefRefPtr<CefDownloadItemCallback> obj)
      : ScopedJNIObject<CefDownloadItemCallback>(
            env,
            obj,
            "org/cef/callback/CefDownloadItemCallback_N",
            "CefDownloadItemCallback") {}
};

}  // namespace

DownloadHandler::DownloadHandler(JNIEnv* env, jobject handler)
    : handle_(env, handler) {}

bool DownloadHandler::OnBeforeDownload(
    CefRefPtr<CefBrowser> browser,
    CefRefPtr<CefDownloadItem> download_item,
    const CefString& suggested_name,
    CefRefPtr<CefBeforeDownloadCallback> callback) {
  ScopedJNIEnv env;
  if (!env)
    return false;

  ScopedJNIBrowser jbrowser(env, browser);
  ScopedJNIDownloadItem jdownloadItem(env, download_item);
  jdownloadItem.SetTemporary();
  ScopedJNIString jsuggestedName(env, suggested_name);
  ScopedJNIBeforeDownloadCallback jcallback(env, callback);

  jboolean jresult = 0;

  JNI_CALL_BOOLEAN_METHOD(
      jresult, env, handle_, "onBeforeDownload",
      "(Lorg/cef/browser/CefBrowser;Lorg/cef/callback/CefDownloadItem;"
      "Ljava/lang/String;Lorg/cef/callback/CefBeforeDownloadCallback;)Z",
      jbrowser.get(), jdownloadItem.get(), jsuggestedName.get(),
      jcallback.get());

  return jresult;
}

void DownloadHandler::OnDownloadUpdated(
    CefRefPtr<CefBrowser> browser,
    CefRefPtr<CefDownloadItem> download_item,
    CefRefPtr<CefDownloadItemCallback> callback) {
  ScopedJNIEnv env;
  if (!env)
    return;

  ScopedJNIBrowser jbrowser(env, browser);
  ScopedJNIDownloadItem jdownloadItem(env, download_item);
  jdownloadItem.SetTemporary();
  ScopedJNIDownloadItemCallback jcallback(env, callback);

  JNI_CALL_VOID_METHOD(
      env, handle_, "onDownloadUpdated",
      "(Lorg/cef/browser/CefBrowser;Lorg/cef/callback/CefDownloadItem;"
      "Lorg/cef/callback/CefDownloadItemCallback;)V",
      jbrowser.get(), jdownloadItem.get(), jcallback.get());
}
