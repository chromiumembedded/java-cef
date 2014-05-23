// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "CefDownloadItem_N.h"
#include "include/cef_download_item.h"
#include "jni_util.h"

JNIEXPORT jboolean JNICALL Java_org_cef_callback_CefDownloadItem_1N_N_1IsValid
  (JNIEnv *env, jobject obj) {
  CefRefPtr<CefDownloadItem> downloadItem =
      GetCefFromJNIObject<CefDownloadItem>(env, obj, "CefDownloadItem");
  if (!downloadItem.get())
    return JNI_FALSE;
  return downloadItem->IsValid() ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jboolean JNICALL Java_org_cef_callback_CefDownloadItem_1N_N_1IsInProgress
  (JNIEnv *env, jobject obj) {
  CefRefPtr<CefDownloadItem> downloadItem =
      GetCefFromJNIObject<CefDownloadItem>(env, obj, "CefDownloadItem");
  if (!downloadItem.get())
    return JNI_FALSE;
  return downloadItem->IsInProgress() ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jboolean JNICALL Java_org_cef_callback_CefDownloadItem_1N_N_1IsComplete
  (JNIEnv *env, jobject obj) {
  CefRefPtr<CefDownloadItem> downloadItem =
      GetCefFromJNIObject<CefDownloadItem>(env, obj, "CefDownloadItem");
  if (!downloadItem.get())
    return JNI_FALSE;
  return downloadItem->IsComplete() ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jboolean JNICALL Java_org_cef_callback_CefDownloadItem_1N_N_1IsCanceled
  (JNIEnv *env, jobject obj) {
  CefRefPtr<CefDownloadItem> downloadItem =
      GetCefFromJNIObject<CefDownloadItem>(env, obj, "CefDownloadItem");
  if (!downloadItem.get())
    return JNI_FALSE;
  return downloadItem->IsCanceled() ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jlong JNICALL Java_org_cef_callback_CefDownloadItem_1N_N_1GetCurrentSpeed
  (JNIEnv *env, jobject obj) {
  CefRefPtr<CefDownloadItem> downloadItem =
      GetCefFromJNIObject<CefDownloadItem>(env, obj, "CefDownloadItem");
  if (!downloadItem.get())
    return 0;
  return downloadItem->GetCurrentSpeed();
}

JNIEXPORT jint JNICALL Java_org_cef_callback_CefDownloadItem_1N_N_1GetPercentComplete
  (JNIEnv *env, jobject obj) {
  CefRefPtr<CefDownloadItem> downloadItem =
      GetCefFromJNIObject<CefDownloadItem>(env, obj, "CefDownloadItem");
  if (!downloadItem.get())
    return 0;
  return downloadItem->GetPercentComplete();
}

JNIEXPORT jlong JNICALL Java_org_cef_callback_CefDownloadItem_1N_N_1GetTotalBytes
  (JNIEnv *env, jobject obj) {
  CefRefPtr<CefDownloadItem> downloadItem =
      GetCefFromJNIObject<CefDownloadItem>(env, obj, "CefDownloadItem");
  if (!downloadItem.get())
    return 0;
  return downloadItem->GetTotalBytes();
}

JNIEXPORT jlong JNICALL Java_org_cef_callback_CefDownloadItem_1N_N_1GetReceivedBytes
  (JNIEnv *env, jobject obj) {
  CefRefPtr<CefDownloadItem> downloadItem =
      GetCefFromJNIObject<CefDownloadItem>(env, obj, "CefDownloadItem");
  if (!downloadItem.get())
    return 0;
  return downloadItem->GetReceivedBytes();
}

JNIEXPORT jobject JNICALL Java_org_cef_callback_CefDownloadItem_1N_N_1GetStartTime
  (JNIEnv *env, jobject obj) {
  CefRefPtr<CefDownloadItem> downloadItem =
      GetCefFromJNIObject<CefDownloadItem>(env, obj, "CefDownloadItem");
  if (!downloadItem.get())
    return NULL;
  CefTime start = downloadItem->GetStartTime();
  return NewJNIDate(env, start);
}

JNIEXPORT jobject JNICALL Java_org_cef_callback_CefDownloadItem_1N_N_1GetEndTime
  (JNIEnv *env, jobject obj) {
  CefRefPtr<CefDownloadItem> downloadItem =
      GetCefFromJNIObject<CefDownloadItem>(env, obj, "CefDownloadItem");
  if (!downloadItem.get())
    return NULL;
  CefTime end = downloadItem->GetEndTime();
  return NewJNIDate(env, end);
}

JNIEXPORT jstring JNICALL Java_org_cef_callback_CefDownloadItem_1N_N_1GetFullPath
  (JNIEnv *env, jobject obj) {
  CefRefPtr<CefDownloadItem> downloadItem =
      GetCefFromJNIObject<CefDownloadItem>(env, obj, "CefDownloadItem");
  if (!downloadItem.get())
    return NULL;
  return NewJNIString(env, downloadItem->GetFullPath());
}

JNIEXPORT jint JNICALL Java_org_cef_callback_CefDownloadItem_1N_N_1GetId
  (JNIEnv *env, jobject obj) {
  CefRefPtr<CefDownloadItem> downloadItem =
      GetCefFromJNIObject<CefDownloadItem>(env, obj, "CefDownloadItem");
  if (!downloadItem.get())
    return -1;
  return downloadItem->GetId();
}

JNIEXPORT jstring JNICALL Java_org_cef_callback_CefDownloadItem_1N_N_1GetURL
  (JNIEnv *env, jobject obj) {
  CefRefPtr<CefDownloadItem> downloadItem =
      GetCefFromJNIObject<CefDownloadItem>(env, obj, "CefDownloadItem");
  if (!downloadItem.get())
    return NULL;
  return NewJNIString(env, downloadItem->GetURL());
}

JNIEXPORT jstring JNICALL Java_org_cef_callback_CefDownloadItem_1N_N_1GetSuggestedFileName
  (JNIEnv *env, jobject obj) {
  CefRefPtr<CefDownloadItem> downloadItem =
      GetCefFromJNIObject<CefDownloadItem>(env, obj, "CefDownloadItem");
  if (!downloadItem.get())
    return NULL;
  return NewJNIString(env, downloadItem->GetSuggestedFileName());
}

JNIEXPORT jstring JNICALL Java_org_cef_callback_CefDownloadItem_1N_N_1GetContentDisposition
  (JNIEnv *env, jobject obj) {
  CefRefPtr<CefDownloadItem> downloadItem =
      GetCefFromJNIObject<CefDownloadItem>(env, obj, "CefDownloadItem");
  if (!downloadItem.get())
    return NULL;
  return NewJNIString(env, downloadItem->GetContentDisposition());
}

JNIEXPORT jstring JNICALL Java_org_cef_callback_CefDownloadItem_1N_N_1GetMimeType
  (JNIEnv *env, jobject obj) {
  CefRefPtr<CefDownloadItem> downloadItem =
      GetCefFromJNIObject<CefDownloadItem>(env, obj, "CefDownloadItem");
  if (!downloadItem.get())
    return NULL;
  return NewJNIString(env, downloadItem->GetMimeType());
}
