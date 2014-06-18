// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "CefPostDataElement_N.h"
#include "include/cef_request.h"
#include "jni_util.h"

JNIEXPORT void JNICALL Java_org_cef_network_CefPostDataElement_1N_N_1CefPostDataElement_1CTOR
  (JNIEnv *env, jobject obj) {
  CefRefPtr<CefPostDataElement> dataElement = CefPostDataElement::Create();
  if (!dataElement.get())
    return;
  SetCefForJNIObject(env, obj, dataElement.get(), "CefPostDataElement");
}

JNIEXPORT jboolean JNICALL Java_org_cef_network_CefPostDataElement_1N_N_1IsReadOnly
  (JNIEnv *env, jobject obj) {
  CefRefPtr<CefPostDataElement> dataElement =
      GetCefFromJNIObject<CefPostDataElement>(env, obj, "CefPostDataElement");
  if (!dataElement)
    return JNI_FALSE;
  return dataElement->IsReadOnly() ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT void JNICALL Java_org_cef_network_CefPostDataElement_1N_N_1SetToEmpty
  (JNIEnv *env, jobject obj) {
  CefRefPtr<CefPostDataElement> dataElement =
      GetCefFromJNIObject<CefPostDataElement>(env, obj, "CefPostDataElement");
  if (!dataElement)
    return;
  dataElement->SetToEmpty();
}

JNIEXPORT void JNICALL Java_org_cef_network_CefPostDataElement_1N_N_1SetToFile
  (JNIEnv *env, jobject obj, jstring jfilename) {
  CefRefPtr<CefPostDataElement> dataElement =
      GetCefFromJNIObject<CefPostDataElement>(env, obj, "CefPostDataElement");
  if (!dataElement)
    return;
  dataElement->SetToFile(GetJNIString(env, jfilename));
}

JNIEXPORT void JNICALL Java_org_cef_network_CefPostDataElement_1N_N_1SetToBytes
  (JNIEnv *env, jobject obj, jint jsize, jbyteArray jbytes) {
  CefRefPtr<CefPostDataElement> dataElement =
      GetCefFromJNIObject<CefPostDataElement>(env, obj, "CefPostDataElement");
  if (!dataElement)
    return;

  jbyte* jbyte = env->GetByteArrayElements(jbytes, NULL);
  if (!jbyte)
    return;
  dataElement->SetToBytes(jsize, jbyte);
  env->ReleaseByteArrayElements(jbytes, jbyte, 0);
}

JNIEXPORT jobject JNICALL Java_org_cef_network_CefPostDataElement_1N_N_1GetType
  (JNIEnv *env, jobject obj) {
  CefRefPtr<CefPostDataElement> dataElement =
      GetCefFromJNIObject<CefPostDataElement>(env, obj, "CefPostDataElement");
  if (!dataElement)
    return NULL;

  CefPostDataElement::Type type = dataElement->GetType();
  jobject jtype = NULL;
  switch (type) {
    default:
    JNI_CASE(env, "org/cef/network/CefPostDataElement$Type", PDE_TYPE_EMPTY, jtype);
    JNI_CASE(env, "org/cef/network/CefPostDataElement$Type", PDE_TYPE_BYTES, jtype);
    JNI_CASE(env, "org/cef/network/CefPostDataElement$Type", PDE_TYPE_FILE,  jtype);
  }
  return jtype;
}

JNIEXPORT jstring JNICALL Java_org_cef_network_CefPostDataElement_1N_N_1GetFile
  (JNIEnv *env, jobject obj) {
  CefRefPtr<CefPostDataElement> dataElement =
      GetCefFromJNIObject<CefPostDataElement>(env, obj, "CefPostDataElement");
  if (!dataElement)
    return NULL;
  return NewJNIString(env, dataElement->GetFile());
}

JNIEXPORT jint JNICALL Java_org_cef_network_CefPostDataElement_1N_N_1GetBytesCount
  (JNIEnv *env, jobject obj) {
  CefRefPtr<CefPostDataElement> dataElement =
      GetCefFromJNIObject<CefPostDataElement>(env, obj, "CefPostDataElement");
  if (!dataElement)
    return 0;
  return (jint)dataElement->GetBytesCount();
}

JNIEXPORT jint JNICALL Java_org_cef_network_CefPostDataElement_1N_N_1GetBytes
  (JNIEnv *env, jobject obj, jint jsize, jbyteArray jbytes) {
  CefRefPtr<CefPostDataElement> dataElement =
      GetCefFromJNIObject<CefPostDataElement>(env, obj, "CefPostDataElement");
  if (!dataElement)
    return 0;

  jbyte* jbyte = env->GetByteArrayElements(jbytes, NULL);
  if (!jbyte)
    return 0;
  memset(jbyte, 0, jsize);

  size_t readLen = dataElement->GetBytes(jsize, jbyte);
  env->ReleaseByteArrayElements(jbytes, jbyte, 0);
  return (jint)readLen;
}

JNIEXPORT void JNICALL Java_org_cef_network_CefPostDataElement_1N_N_1CefPostDataElement_1DTOR
  (JNIEnv *env, jobject obj) {
  SetCefForJNIObject<CefPostDataElement>(env, obj, NULL, "CefPostDataElement");
}
