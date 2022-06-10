// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "CefPostData_N.h"

#include "include/cef_request.h"

#include "jni_scoped_helpers.h"
#include "jni_util.h"

namespace {

const char kCefClassName[] = "CefPostData";

CefRefPtr<CefPostData> GetSelf(jlong self) {
  return reinterpret_cast<CefPostData*>(self);
}

}  // namespace

JNIEXPORT jobject JNICALL
Java_org_cef_network_CefPostData_1N_N_1Create(JNIEnv* env, jclass cls) {
  CefRefPtr<CefPostData> postData = CefPostData::Create();
  ScopedJNIPostData jpostData(env, postData);
  return jpostData.Release();
}

JNIEXPORT void JNICALL
Java_org_cef_network_CefPostData_1N_N_1Dispose(JNIEnv* env,
                                               jobject obj,
                                               jlong self) {
  SetCefForJNIObject<CefPostData>(env, obj, nullptr, kCefClassName);
}

JNIEXPORT jboolean JNICALL
Java_org_cef_network_CefPostData_1N_N_1IsReadOnly(JNIEnv* env,
                                                  jobject obj,
                                                  jlong self) {
  CefRefPtr<CefPostData> postData = GetSelf(self);
  if (!postData)
    return JNI_FALSE;
  return postData->IsReadOnly() ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jint JNICALL
Java_org_cef_network_CefPostData_1N_N_1GetElementCount(JNIEnv* env,
                                                       jobject obj,
                                                       jlong self) {
  CefRefPtr<CefPostData> postData = GetSelf(self);
  if (!postData)
    return 0;
  return (jint)postData->GetElementCount();
}

JNIEXPORT void JNICALL
Java_org_cef_network_CefPostData_1N_N_1GetElements(JNIEnv* env,
                                                   jobject obj,
                                                   jlong self,
                                                   jobject jelements) {
  CefRefPtr<CefPostData> postData = GetSelf(self);
  if (!postData)
    return;
  CefPostData::ElementVector elements;
  postData->GetElements(elements);

  CefPostData::ElementVector::const_iterator iter;
  for (iter = elements.begin(); iter != elements.end(); ++iter) {
    ScopedJNIPostDataElement jdataElement(env, *iter);
    JNI_CALL_VOID_METHOD(env, jelements, "addElement", "(Ljava/lang/Object;)V",
                         jdataElement.get());
  }
}

JNIEXPORT jboolean JNICALL
Java_org_cef_network_CefPostData_1N_N_1RemoveElement(JNIEnv* env,
                                                     jobject obj,
                                                     jlong self,
                                                     jobject jelement) {
  CefRefPtr<CefPostData> postData = GetSelf(self);
  if (!postData)
    return JNI_FALSE;

  ScopedJNIPostDataElement dataElementObj(env);
  dataElementObj.SetHandle(jelement, false /* should_delete */);

  CefRefPtr<CefPostDataElement> dataElement = dataElementObj.GetCefObject();
  if (!dataElement)
    return JNI_FALSE;

  return postData->RemoveElement(dataElement) ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jboolean JNICALL
Java_org_cef_network_CefPostData_1N_N_1AddElement(JNIEnv* env,
                                                  jobject obj,
                                                  jlong self,
                                                  jobject jelement) {
  CefRefPtr<CefPostData> postData = GetSelf(self);
  if (!postData)
    return JNI_FALSE;

  ScopedJNIPostDataElement dataElementObj(env);
  dataElementObj.SetHandle(jelement, false /* should_delete */);

  CefRefPtr<CefPostDataElement> dataElement = dataElementObj.GetCefObject();
  if (!dataElement)
    return JNI_FALSE;

  return postData->AddElement(dataElement) ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT void JNICALL
Java_org_cef_network_CefPostData_1N_N_1RemoveElements(JNIEnv* env,
                                                      jobject obj,
                                                      jlong self) {
  CefRefPtr<CefPostData> postData = GetSelf(self);
  if (!postData)
    return;
  postData->RemoveElements();
}
