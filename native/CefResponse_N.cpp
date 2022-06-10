// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "CefResponse_N.h"

#include "include/cef_response.h"

#include "jni_scoped_helpers.h"
#include "jni_util.h"

namespace {

const char kCefClassName[] = "CefResponse";

CefRefPtr<CefResponse> GetSelf(jlong self) {
  return reinterpret_cast<CefResponse*>(self);
}

}  // namespace

JNIEXPORT jobject JNICALL
Java_org_cef_network_CefResponse_1N_N_1Create(JNIEnv* env, jclass cls) {
  CefRefPtr<CefResponse> response = CefResponse::Create();
  ScopedJNIResponse jresponse(env, response);
  return jresponse.Release();
}

JNIEXPORT void JNICALL
Java_org_cef_network_CefResponse_1N_N_1Dispose(JNIEnv* env,
                                               jobject obj,
                                               jlong self) {
  SetCefForJNIObject<CefResponse>(env, obj, nullptr, kCefClassName);
}

JNIEXPORT jboolean JNICALL
Java_org_cef_network_CefResponse_1N_N_1IsReadOnly(JNIEnv* env,
                                                  jobject obj,
                                                  jlong self) {
  CefRefPtr<CefResponse> response = GetSelf(self);
  if (!response)
    return JNI_FALSE;
  return response->IsReadOnly() ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jobject JNICALL
Java_org_cef_network_CefResponse_1N_N_1GetError(JNIEnv* env,
                                                jobject obj,
                                                jlong self) {
  CefRefPtr<CefResponse> response = GetSelf(self);
  if (!response)
    return nullptr;
  return NewJNIErrorCode(env, response->GetError());
}

JNIEXPORT void JNICALL
Java_org_cef_network_CefResponse_1N_N_1SetError(JNIEnv* env,
                                                jobject obj,
                                                jlong self,
                                                jobject jerrorCode) {
  CefRefPtr<CefResponse> response = GetSelf(self);
  if (!response)
    return;

  if (!jerrorCode)
    return;
  response->SetError(GetJNIErrorCode(env, jerrorCode));
}

JNIEXPORT jint JNICALL
Java_org_cef_network_CefResponse_1N_N_1GetStatus(JNIEnv* env,
                                                 jobject obj,
                                                 jlong self) {
  CefRefPtr<CefResponse> response = GetSelf(self);
  if (!response)
    return 0;
  return response->GetStatus();
}

JNIEXPORT void JNICALL
Java_org_cef_network_CefResponse_1N_N_1SetStatus(JNIEnv* env,
                                                 jobject obj,
                                                 jlong self,
                                                 jint jstatus) {
  CefRefPtr<CefResponse> response = GetSelf(self);
  if (!response)
    return;
  return response->SetStatus(jstatus);
}

JNIEXPORT jstring JNICALL
Java_org_cef_network_CefResponse_1N_N_1GetStatusText(JNIEnv* env,
                                                     jobject obj,
                                                     jlong self) {
  CefRefPtr<CefResponse> response = GetSelf(self);
  if (!response)
    return nullptr;
  return NewJNIString(env, response->GetStatusText());
}

JNIEXPORT void JNICALL
Java_org_cef_network_CefResponse_1N_N_1SetStatusText(JNIEnv* env,
                                                     jobject obj,
                                                     jlong self,
                                                     jstring jstatus) {
  CefRefPtr<CefResponse> response = GetSelf(self);
  if (!response)
    return;
  response->SetStatusText(GetJNIString(env, jstatus));
}

JNIEXPORT jstring JNICALL
Java_org_cef_network_CefResponse_1N_N_1GetMimeType(JNIEnv* env,
                                                   jobject obj,
                                                   jlong self) {
  CefRefPtr<CefResponse> response = GetSelf(self);
  if (!response)
    return nullptr;
  return NewJNIString(env, response->GetMimeType());
}

JNIEXPORT void JNICALL
Java_org_cef_network_CefResponse_1N_N_1SetMimeType(JNIEnv* env,
                                                   jobject obj,
                                                   jlong self,
                                                   jstring jmimeType) {
  CefRefPtr<CefResponse> response = GetSelf(self);
  if (!response)
    return;
  response->SetMimeType(GetJNIString(env, jmimeType));
}

JNIEXPORT jstring JNICALL
Java_org_cef_network_CefResponse_1N_N_1GetHeaderByName(JNIEnv* env,
                                                       jobject obj,
                                                       jlong self,
                                                       jstring jname) {
  CefRefPtr<CefResponse> response = GetSelf(self);
  if (!response)
    return nullptr;
  return NewJNIString(env, response->GetHeaderByName(GetJNIString(env, jname)));
}

JNIEXPORT void JNICALL
Java_org_cef_network_CefResponse_1N_N_1SetHeaderByName(JNIEnv* env,
                                                       jobject obj,
                                                       jlong self,
                                                       jstring jname,
                                                       jstring jvalue,
                                                       jboolean joverride) {
  CefRefPtr<CefResponse> response = GetSelf(self);
  if (!response)
    return;
  return response->SetHeaderByName(GetJNIString(env, jname),
                                   GetJNIString(env, jvalue),
                                   joverride != JNI_FALSE);
}

JNIEXPORT void JNICALL
Java_org_cef_network_CefResponse_1N_N_1GetHeaderMap(JNIEnv* env,
                                                    jobject obj,
                                                    jlong self,
                                                    jobject jheaderMap) {
  CefRefPtr<CefResponse> response = GetSelf(self);
  if (!response)
    return;

  CefResponse::HeaderMap headerMap;
  response->GetHeaderMap(headerMap);
  SetJNIStringMultiMap(env, jheaderMap, headerMap);
}

JNIEXPORT void JNICALL
Java_org_cef_network_CefResponse_1N_N_1SetHeaderMap(JNIEnv* env,
                                                    jobject obj,
                                                    jlong self,
                                                    jobject jheaderMap) {
  CefRefPtr<CefResponse> response = GetSelf(self);
  if (!response)
    return;

  CefResponse::HeaderMap headerMap;
  GetJNIStringMultiMap(env, jheaderMap, headerMap);
  response->SetHeaderMap(headerMap);
}
