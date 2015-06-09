// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "CefResponse_N.h"
#include "include/cef_response.h"
#include "jni_util.h"

JNIEXPORT void JNICALL Java_org_cef_network_CefResponse_1N_N_1CefResponse_1CTOR
  (JNIEnv *env, jobject obj) {
  CefRefPtr<CefResponse> response = CefResponse::Create();
  if (!response.get())
    return;
  SetCefForJNIObject(env, obj, response.get(), "CefResponse");
}

JNIEXPORT jboolean JNICALL Java_org_cef_network_CefResponse_1N_N_1IsReadOnly
  (JNIEnv *env, jobject obj) {
  CefRefPtr<CefResponse> response = GetCefFromJNIObject<CefResponse>(env, obj, "CefResponse");
  if (!response)
    return JNI_FALSE;
  return response->IsReadOnly() ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jint JNICALL Java_org_cef_network_CefResponse_1N_N_1GetStatus
  (JNIEnv *env, jobject obj) {
  CefRefPtr<CefResponse> response = GetCefFromJNIObject<CefResponse>(env, obj, "CefResponse");
  if (!response)
    return 0;
  return response->GetStatus();
}

JNIEXPORT void JNICALL Java_org_cef_network_CefResponse_1N_N_1SetStatus
  (JNIEnv *env, jobject obj, jint jstatus) {
  CefRefPtr<CefResponse> response = GetCefFromJNIObject<CefResponse>(env, obj, "CefResponse");
  if (!response)
    return;
  return response->SetStatus(jstatus);
}

JNIEXPORT jstring JNICALL Java_org_cef_network_CefResponse_1N_N_1GetStatusText
  (JNIEnv *env, jobject obj) {
  CefRefPtr<CefResponse> response = GetCefFromJNIObject<CefResponse>(env, obj, "CefResponse");
  if (!response)
    return NULL;
  return NewJNIString(env, response->GetStatusText());
}

JNIEXPORT void JNICALL Java_org_cef_network_CefResponse_1N_N_1SetStatusText
  (JNIEnv *env, jobject obj, jstring jstatus) {
  CefRefPtr<CefResponse> response = GetCefFromJNIObject<CefResponse>(env, obj, "CefResponse");
  if (!response)
    return;
  response->SetStatusText(GetJNIString(env, jstatus));
}

JNIEXPORT jstring JNICALL Java_org_cef_network_CefResponse_1N_N_1GetMimeType
  (JNIEnv *env, jobject obj) {
  CefRefPtr<CefResponse> response = GetCefFromJNIObject<CefResponse>(env, obj, "CefResponse");
  if (!response)
    return NULL;
  return NewJNIString(env, response->GetMimeType());
}

JNIEXPORT void JNICALL Java_org_cef_network_CefResponse_1N_N_1SetMimeType
  (JNIEnv *env, jobject obj, jstring jmimeType) {
  CefRefPtr<CefResponse> response = GetCefFromJNIObject<CefResponse>(env, obj, "CefResponse");
  if (!response)
    return;
  response->SetMimeType(GetJNIString(env, jmimeType));
}

JNIEXPORT jstring JNICALL Java_org_cef_network_CefResponse_1N_N_1GetHeader
  (JNIEnv *env, jobject obj, jstring jheader) {
  CefRefPtr<CefResponse> response = GetCefFromJNIObject<CefResponse>(env, obj, "CefResponse");
  if (!response)
    return NULL;
  return NewJNIString(env, response->GetHeader(GetJNIString(env, jheader)));
}

JNIEXPORT void JNICALL Java_org_cef_network_CefResponse_1N_N_1GetHeaderMap
  (JNIEnv *env, jobject obj, jobject jheaderMap) {
  CefRefPtr<CefResponse> response = GetCefFromJNIObject<CefResponse>(env, obj, "CefResponse");
  if (!response)
    return;  
  CefResponse::HeaderMap headerMap;
  response->GetHeaderMap(headerMap);

  for (CefResponse::HeaderMap::iterator iter = headerMap.begin(); iter != headerMap.end(); ++iter) {
    jstring jkey = NewJNIString(env, iter->first);
    jstring jvalue = NewJNIString(env, iter->second);
    jobject returnIgn = NULL;
    JNI_CALL_METHOD(env,
                    jheaderMap,
                    "put",
                    "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;",
                    Object,
                    returnIgn,
                    jkey,
                    jvalue);
    UNUSED(returnIgn);
  }
}

JNIEXPORT void JNICALL Java_org_cef_network_CefResponse_1N_N_1SetHeaderMap
  (JNIEnv *env, jobject obj, jobject jheaderMap) {
  CefRefPtr<CefResponse> response = GetCefFromJNIObject<CefResponse>(env, obj, "CefResponse");
  if (!response)
    return;

  // public abstract java.util.Set<java.util.Map$Entry<K, V>> entrySet();
  jobject entrySet = NULL;
  JNI_CALL_METHOD(env, jheaderMap,
                  "entrySet",
                  "()Ljava/util/Set;",
                  Object,
                  entrySet);
  if (!entrySet)
    return;

  // public abstract java.lang.Object[] toArray();
  jobject entrySetValues = NULL;
  JNI_CALL_METHOD(env, entrySet,
                  "toArray",
                  "()[Ljava/lang/Object;",
                  Object,
                  entrySetValues);
  if (!entrySetValues)
    return;

  CefResponse::HeaderMap headerMap;
  jint length = env->GetArrayLength((jobjectArray)entrySetValues);
  for (jint i=0; i < length; i++) {
    jobject mapEntry = env->GetObjectArrayElement((jobjectArray)entrySetValues, i);
    if (!mapEntry)
      return;
    jobject key = NULL;
    jobject value = NULL;
    JNI_CALL_METHOD(env, mapEntry,
                    "getKey",
                    "()Ljava/lang/Object;",
                    Object,
                    key);
    JNI_CALL_METHOD(env, mapEntry,
                    "getValue",
                    "()Ljava/lang/Object;",
                    Object,
                    value);
    headerMap.insert(std::make_pair(GetJNIString(env, (jstring)key), GetJNIString(env, (jstring)value)));
  }
  response->SetHeaderMap(headerMap);
}

JNIEXPORT void JNICALL Java_org_cef_network_CefResponse_1N_N_1CefResponse_1DTOR
  (JNIEnv *env, jobject obj) {
  SetCefForJNIObject<CefResponse>(env, obj, NULL, "CefResponse");
}
