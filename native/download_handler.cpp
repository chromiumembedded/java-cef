// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "download_handler.h"

#include "client_handler.h"
#include "jni_util.h"
#include "util.h"

DownloadHandler::DownloadHandler(JNIEnv* env, jobject handler) {
  jhandler_ = env->NewGlobalRef(handler);
}

DownloadHandler::~DownloadHandler() {
  JNIEnv* env = GetJNIEnv();
  env->DeleteGlobalRef(jhandler_);
}

void DownloadHandler::OnBeforeDownload(
    CefRefPtr<CefBrowser> browser,
    CefRefPtr<CefDownloadItem> download_item,
    const CefString& suggested_name,
    CefRefPtr<CefBeforeDownloadCallback> callback) {
  JNIEnv* env = GetJNIEnv();
  if (!env)
    return;

  jobject jdownloadItem =
      NewJNIObject(env, "org/cef/callback/CefDownloadItem_N");
  if (!jdownloadItem)
    return;
  SetCefForJNIObject(env, jdownloadItem, download_item.get(),
                     "CefDownloadItem");

  jobject jcallback =
      NewJNIObject(env, "org/cef/callback/CefBeforeDownloadCallback_N");
  if (!jcallback) {
    env->DeleteLocalRef(jdownloadItem);
    return;
  }
  SetCefForJNIObject(env, jcallback, callback.get(),
                     "CefBeforeDownloadCallback");

  jstring jsuggested_name = NewJNIString(env, suggested_name);
  JNI_CALL_VOID_METHOD(
      env, jhandler_, "onBeforeDownload",
      "(Lorg/cef/browser/CefBrowser;Lorg/cef/callback/CefDownloadItem;"
      "Ljava/lang/String;Lorg/cef/callback/CefBeforeDownloadCallback;)V",
      GetJNIBrowser(browser), jdownloadItem, jsuggested_name, jcallback);

  // delete CefDownloadItem reference from Java because the object
  // is only valid within this call
  SetCefForJNIObject<CefDownloadItem>(env, jdownloadItem, NULL,
                                      "CefDownloadItem");
  env->DeleteLocalRef(jdownloadItem);
  env->DeleteLocalRef(jcallback);
  env->DeleteLocalRef(jsuggested_name);
}

void DownloadHandler::OnDownloadUpdated(
    CefRefPtr<CefBrowser> browser,
    CefRefPtr<CefDownloadItem> download_item,
    CefRefPtr<CefDownloadItemCallback> callback) {
  JNIEnv* env = GetJNIEnv();
  if (!env)
    return;

  jobject jdownloadItem =
      NewJNIObject(env, "org/cef/callback/CefDownloadItem_N");
  if (!jdownloadItem)
    return;
  SetCefForJNIObject(env, jdownloadItem, download_item.get(),
                     "CefDownloadItem");

  jobject jcallback =
      NewJNIObject(env, "org/cef/callback/CefDownloadItemCallback_N");
  if (!jcallback) {
    env->DeleteLocalRef(jdownloadItem);
    return;
  }
  SetCefForJNIObject(env, jcallback, callback.get(), "CefDownloadItemCallback");

  JNI_CALL_VOID_METHOD(
      env, jhandler_, "onDownloadUpdated",
      "(Lorg/cef/browser/CefBrowser;Lorg/cef/callback/CefDownloadItem;"
      "Lorg/cef/callback/CefDownloadItemCallback;)V",
      GetJNIBrowser(browser), jdownloadItem, jcallback);

  // delete CefDownloadItem reference from Java because the object
  // is only valid within this call
  SetCefForJNIObject<CefDownloadItem>(env, jdownloadItem, NULL,
                                      "CefDownloadItem");
  env->DeleteLocalRef(jdownloadItem);
  env->DeleteLocalRef(jcallback);
}
