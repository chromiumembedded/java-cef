// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "CefDragData_N.h"
#include "include/cef_drag_handler.h"
#include "jni_util.h"

JNIEXPORT jboolean JNICALL Java_org_cef_callback_CefDragData_1N_N_1IsLink
  (JNIEnv *env, jobject obj) {
  CefRefPtr<CefDragData> dragData =
      GetCefFromJNIObject<CefDragData>(env, obj, "CefDragData");
  if (!dragData.get())
    return JNI_FALSE;
  return dragData->IsLink() ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jboolean JNICALL Java_org_cef_callback_CefDragData_1N_N_1IsFragment
  (JNIEnv *env, jobject obj) {
  CefRefPtr<CefDragData> dragData =
      GetCefFromJNIObject<CefDragData>(env, obj, "CefDragData");
  if (!dragData.get())
    return JNI_FALSE;
  return dragData->IsFragment() ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jboolean JNICALL Java_org_cef_callback_CefDragData_1N_N_1IsFile
  (JNIEnv *env, jobject obj) {
  CefRefPtr<CefDragData> dragData =
      GetCefFromJNIObject<CefDragData>(env, obj, "CefDragData");
  if (!dragData.get())
    return JNI_FALSE;
  return dragData->IsFile() ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jstring JNICALL Java_org_cef_callback_CefDragData_1N_N_1GetLinkURL
  (JNIEnv *env, jobject obj) {
  CefRefPtr<CefDragData> dragData =
      GetCefFromJNIObject<CefDragData>(env, obj, "CefDragData");
  if (!dragData.get())
    return NULL;
  return NewJNIString(env, dragData->GetLinkURL());
}

JNIEXPORT jstring JNICALL Java_org_cef_callback_CefDragData_1N_N_1GetLinkTitle
  (JNIEnv *env, jobject obj) {
  CefRefPtr<CefDragData> dragData =
      GetCefFromJNIObject<CefDragData>(env, obj, "CefDragData");
  if (!dragData.get())
    return NULL;
  return NewJNIString(env, dragData->GetLinkTitle());
}

JNIEXPORT jstring JNICALL Java_org_cef_callback_CefDragData_1N_N_1GetLinkMetadata
  (JNIEnv *env, jobject obj) {
  CefRefPtr<CefDragData> dragData =
      GetCefFromJNIObject<CefDragData>(env, obj, "CefDragData");
  if (!dragData.get())
    return NULL;
  return NewJNIString(env, dragData->GetLinkMetadata());
}

JNIEXPORT jstring JNICALL Java_org_cef_callback_CefDragData_1N_N_1GetFragmentText
  (JNIEnv *env, jobject obj) {
  CefRefPtr<CefDragData> dragData =
      GetCefFromJNIObject<CefDragData>(env, obj, "CefDragData");
  if (!dragData.get())
    return NULL;
  return NewJNIString(env, dragData->GetFragmentText());
}

JNIEXPORT jstring JNICALL Java_org_cef_callback_CefDragData_1N_N_1GetFragmentHtml
  (JNIEnv *env, jobject obj) {
  CefRefPtr<CefDragData> dragData =
      GetCefFromJNIObject<CefDragData>(env, obj, "CefDragData");
  if (!dragData.get())
    return NULL;
  return NewJNIString(env, dragData->GetFragmentHtml());
}

JNIEXPORT jstring JNICALL Java_org_cef_callback_CefDragData_1N_N_1GetFragmentBaseURL
  (JNIEnv *env, jobject obj) {
  CefRefPtr<CefDragData> dragData =
      GetCefFromJNIObject<CefDragData>(env, obj, "CefDragData");
  if (!dragData.get())
    return NULL;
  return NewJNIString(env, dragData->GetFragmentBaseURL());
}

JNIEXPORT jstring JNICALL Java_org_cef_callback_CefDragData_1N_N_1GetFileName
  (JNIEnv *env, jobject obj) {
  CefRefPtr<CefDragData> dragData =
      GetCefFromJNIObject<CefDragData>(env, obj, "CefDragData");
  if (!dragData.get())
    return NULL;
  return NewJNIString(env, dragData->GetFileName());
}

JNIEXPORT jboolean JNICALL Java_org_cef_callback_CefDragData_1N_N_1GetFileNames
  (JNIEnv *env, jobject obj, jobject jfileNames) {
  CefRefPtr<CefDragData> dragData =
      GetCefFromJNIObject<CefDragData>(env, obj, "CefDragData");
  if (!dragData.get())
    return JNI_FALSE;

  std::vector<CefString> fileNames;
  if (!dragData->GetFileNames(fileNames))
    return JNI_FALSE;

  for (size_t i=0; i < fileNames.size(); ++i) {
    AddJNIStringToVector(env, jfileNames, fileNames.at(i));
  }
  return JNI_TRUE;
}
