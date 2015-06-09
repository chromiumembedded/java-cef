// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "CefPostData_N.h"
#include "include/cef_request.h"
#include "jni_util.h"

JNIEXPORT void JNICALL Java_org_cef_network_CefPostData_1N_N_1CefPostData_1CTOR
  (JNIEnv *env, jobject obj) {
  CefRefPtr<CefPostData> postData = CefPostData::Create();
  if (!postData.get())
    return;
  SetCefForJNIObject(env, obj, postData.get(), "CefPostData");
}

JNIEXPORT jboolean JNICALL Java_org_cef_network_CefPostData_1N_N_1IsReadOnly
  (JNIEnv *env, jobject obj) {
  CefRefPtr<CefPostData> postData = GetCefFromJNIObject<CefPostData>(env, obj, "CefPostData");
  if (!postData)
    return JNI_FALSE;
  return postData->IsReadOnly() ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jint JNICALL Java_org_cef_network_CefPostData_1N_N_1GetElementCount
  (JNIEnv *env, jobject obj) {
  CefRefPtr<CefPostData> postData = GetCefFromJNIObject<CefPostData>(env, obj, "CefPostData");
  if (!postData)
    return 0;
  return (jint)postData->GetElementCount();
}

JNIEXPORT void JNICALL Java_org_cef_network_CefPostData_1N_N_1GetElements
  (JNIEnv *env, jobject obj, jobject jelements) {
  CefRefPtr<CefPostData> postData = GetCefFromJNIObject<CefPostData>(env, obj, "CefPostData");
  if (!postData)
    return;
  CefPostData::ElementVector elements;
  postData->GetElements(elements);

  CefPostData::ElementVector::const_iterator iter;
  for (iter = elements.begin(); iter != elements.end(); ++iter) {
    CefRefPtr<CefPostDataElement> dataElement = *iter;
    if (!dataElement.get())
      continue;

    jobject jdataElement = NewJNIObject(env, "org/cef/network/CefPostDataElement_N");
    if (!jdataElement)
      continue;

    SetCefForJNIObject(env, jdataElement, dataElement.get(), "CefPostDataElement");
    JNI_CALL_VOID_METHOD(env, jelements, "addElement", "(Ljava/lang/Object;)V", jdataElement);
  }
}

JNIEXPORT jboolean JNICALL Java_org_cef_network_CefPostData_1N_N_1RemoveElement
  (JNIEnv *env, jobject obj, jobject jelement) {
  CefRefPtr<CefPostData> postData = GetCefFromJNIObject<CefPostData>(env, obj, "CefPostData");
  if (!postData)
    return JNI_FALSE;

  CefRefPtr<CefPostDataElement> dataElement =
      GetCefFromJNIObject<CefPostDataElement>(env, jelement, "CefPostDataElement");
  if (!dataElement.get())
    return JNI_FALSE;

  return postData->RemoveElement(dataElement) ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jboolean JNICALL Java_org_cef_network_CefPostData_1N_N_1AddElement
  (JNIEnv *env, jobject obj, jobject jelement) {
  CefRefPtr<CefPostData> postData = GetCefFromJNIObject<CefPostData>(env, obj, "CefPostData");
  if (!postData)
    return JNI_FALSE;

  CefRefPtr<CefPostDataElement> dataElement =
      GetCefFromJNIObject<CefPostDataElement>(env, jelement, "CefPostDataElement");
  if (!dataElement.get())
    return JNI_FALSE;

  return postData->AddElement(dataElement) ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT void JNICALL Java_org_cef_network_CefPostData_1N_N_1RemoveElements
  (JNIEnv *env, jobject obj) {
  CefRefPtr<CefPostData> postData = GetCefFromJNIObject<CefPostData>(env, obj, "CefPostData");
  if (!postData)
    return;
  postData->RemoveElements();
}

JNIEXPORT void JNICALL Java_org_cef_network_CefPostData_1N_N_1CefPostData_1DTOR
  (JNIEnv *env, jobject obj) {
  SetCefForJNIObject<CefPostData>(env, obj, NULL, "CefPostData");
}

