// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "CefRequest_N.h"
#include "include/cef_request.h"

#include "jni_scoped_helpers.h"
#include "jni_util.h"

namespace {

const char kCefClassName[] = "CefRequest";

CefRefPtr<CefRequest> GetSelf(jlong self) {
  return reinterpret_cast<CefRequest*>(self);
}

}  // namespace

JNIEXPORT jobject JNICALL
Java_org_cef_network_CefRequest_1N_N_1Create(JNIEnv* env, jclass cls) {
  CefRefPtr<CefRequest> request = CefRequest::Create();
  ScopedJNIRequest jrequest(env, request);
  return jrequest.Release();
}

JNIEXPORT void JNICALL
Java_org_cef_network_CefRequest_1N_N_1Dispose(JNIEnv* env,
                                              jobject obj,
                                              jlong self) {
  SetCefForJNIObject<CefRequest>(env, obj, nullptr, kCefClassName);
}

JNIEXPORT jlong JNICALL
Java_org_cef_network_CefRequest_1N_N_1GetIdentifier(JNIEnv* env,
                                                    jobject obj,
                                                    jlong self) {
  CefRefPtr<CefRequest> request = GetSelf(self);
  if (!request)
    return 0;
  return (jlong)request->GetIdentifier();
}

JNIEXPORT jboolean JNICALL
Java_org_cef_network_CefRequest_1N_N_1IsReadOnly(JNIEnv* env,
                                                 jobject obj,
                                                 jlong self) {
  CefRefPtr<CefRequest> request = GetSelf(self);
  if (!request)
    return JNI_FALSE;
  return request->IsReadOnly() ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jstring JNICALL
Java_org_cef_network_CefRequest_1N_N_1GetURL(JNIEnv* env,
                                             jobject obj,
                                             jlong self) {
  CefRefPtr<CefRequest> request = GetSelf(self);
  if (!request)
    return nullptr;
  return NewJNIString(env, request->GetURL());
}

JNIEXPORT void JNICALL
Java_org_cef_network_CefRequest_1N_N_1SetURL(JNIEnv* env,
                                             jobject obj,
                                             jlong self,
                                             jstring jurl) {
  CefRefPtr<CefRequest> request = GetSelf(self);
  if (!request)
    return;
  request->SetURL(GetJNIString(env, jurl));
}

JNIEXPORT void JNICALL
Java_org_cef_network_CefRequest_1N_N_1SetReferrer(JNIEnv* env,
                                                  jobject obj,
                                                  jlong self,
                                                  jstring jurl,
                                                  jobject jpolicy) {
  CefRefPtr<CefRequest> request = GetSelf(self);
  if (!request)
    return;

  cef_referrer_policy_t policy = REFERRER_POLICY_DEFAULT;
  if (jpolicy != nullptr) {
    if (IsJNIEnumValue(env, jpolicy,
                       "org/cef/network/CefRequest$ReferrerPolicy",
                       "REFERRER_POLICY_CLEAR_REFERRER_ON_TRANSITION_FROM_"
                       "SECURE_TO_INSECURE")) {
      policy =
          REFERRER_POLICY_CLEAR_REFERRER_ON_TRANSITION_FROM_SECURE_TO_INSECURE;
    } else if (IsJNIEnumValue(env, jpolicy,
                              "org/cef/network/CefRequest$ReferrerPolicy",
                              "REFERRER_POLICY_REDUCE_REFERRER_GRANULARITY_ON_"
                              "TRANSITION_CROSS_ORIGIN")) {
      policy =
          REFERRER_POLICY_REDUCE_REFERRER_GRANULARITY_ON_TRANSITION_CROSS_ORIGIN;
    } else if (IsJNIEnumValue(
                   env, jpolicy, "org/cef/network/CefRequest$ReferrerPolicy",
                   "REFERRER_POLICY_ORIGIN_ONLY_ON_TRANSITION_CROSS_ORIGIN")) {
      policy = REFERRER_POLICY_ORIGIN_ONLY_ON_TRANSITION_CROSS_ORIGIN;
    } else if (IsJNIEnumValue(env, jpolicy,
                              "org/cef/network/CefRequest$ReferrerPolicy",
                              "REFERRER_POLICY_NEVER_CLEAR_REFERRER")) {
      policy = REFERRER_POLICY_NEVER_CLEAR_REFERRER;
    } else if (IsJNIEnumValue(env, jpolicy,
                              "org/cef/network/CefRequest$ReferrerPolicy",
                              "REFERRER_POLICY_ORIGIN")) {
      policy = REFERRER_POLICY_ORIGIN;
    } else if (IsJNIEnumValue(env, jpolicy,
                              "org/cef/network/CefRequest$ReferrerPolicy",
                              "REFERRER_POLICY_CLEAR_REFERRER_ON_TRANSITION_"
                              "CROSS_ORIGIN")) {
      policy = REFERRER_POLICY_CLEAR_REFERRER_ON_TRANSITION_CROSS_ORIGIN;
    } else if (IsJNIEnumValue(env, jpolicy,
                              "org/cef/network/CefRequest$ReferrerPolicy",
                              "REFERRER_POLICY_ORIGIN_CLEAR_ON_TRANSITION_FROM_"
                              "SECURE_TO_INSECURE")) {
      policy =
          REFERRER_POLICY_ORIGIN_CLEAR_ON_TRANSITION_FROM_SECURE_TO_INSECURE;
    } else if (IsJNIEnumValue(env, jpolicy,
                              "org/cef/network/CefRequest$ReferrerPolicy",
                              "REFERRER_POLICY_NO_REFERRER")) {
      policy = REFERRER_POLICY_NO_REFERRER;
    } else if (IsJNIEnumValue(env, jpolicy,
                              "org/cef/network/CefRequest$ReferrerPolicy",
                              "REFERRER_POLICY_LAST_VALUE")) {
      policy = REFERRER_POLICY_LAST_VALUE;
    }
  }

  request->SetReferrer(GetJNIString(env, jurl), policy);
}

JNIEXPORT jstring JNICALL
Java_org_cef_network_CefRequest_1N_N_1GetReferrerURL(JNIEnv* env,
                                                     jobject obj,
                                                     jlong self) {
  CefRefPtr<CefRequest> request = GetSelf(self);
  if (!request)
    return nullptr;

  return NewJNIString(env, request->GetReferrerURL());
}

JNIEXPORT jobject JNICALL
Java_org_cef_network_CefRequest_1N_N_1GetReferrerPolicy(JNIEnv* env,
                                                        jobject obj,
                                                        jlong self) {
  CefRefPtr<CefRequest> request = GetSelf(self);
  if (!request)
    return nullptr;

  jobject result = nullptr;
  CefRequest::ReferrerPolicy rp = request->GetReferrerPolicy();
  switch (rp) {
    default:
      // same as
      // REFERRER_POLICY_CLEAR_REFERRER_ON_TRANSITION_FROM_SECURE_TO_INSECURE
      JNI_CASE(env, "org/cef/network/CefRequest$ReferrerPolicy",
               REFERRER_POLICY_DEFAULT, result);
      JNI_CASE(
          env, "org/cef/network/CefRequest$ReferrerPolicy",
          REFERRER_POLICY_REDUCE_REFERRER_GRANULARITY_ON_TRANSITION_CROSS_ORIGIN,
          result);
      JNI_CASE(env, "org/cef/network/CefRequest$ReferrerPolicy",
               REFERRER_POLICY_ORIGIN_ONLY_ON_TRANSITION_CROSS_ORIGIN, result);
      JNI_CASE(env, "org/cef/network/CefRequest$ReferrerPolicy",
               REFERRER_POLICY_NEVER_CLEAR_REFERRER, result);
      JNI_CASE(env, "org/cef/network/CefRequest$ReferrerPolicy",
               REFERRER_POLICY_ORIGIN, result);
      JNI_CASE(env, "org/cef/network/CefRequest$ReferrerPolicy",
               REFERRER_POLICY_CLEAR_REFERRER_ON_TRANSITION_CROSS_ORIGIN,
               result);
      JNI_CASE(
          env, "org/cef/network/CefRequest$ReferrerPolicy",
          REFERRER_POLICY_ORIGIN_CLEAR_ON_TRANSITION_FROM_SECURE_TO_INSECURE,
          result);
      JNI_CASE(env, "org/cef/network/CefRequest$ReferrerPolicy",
               REFERRER_POLICY_NO_REFERRER, result);
      // REFERRER_POLICY_LAST_VALUE equals REFERRER_POLICY_NO_REFERRER as of CEF
      // 3683
  }
  return result;
}

JNIEXPORT jstring JNICALL
Java_org_cef_network_CefRequest_1N_N_1GetMethod(JNIEnv* env,
                                                jobject obj,
                                                jlong self) {
  CefRefPtr<CefRequest> request = GetSelf(self);
  if (!request)
    return nullptr;
  return NewJNIString(env, request->GetMethod());
}

JNIEXPORT void JNICALL
Java_org_cef_network_CefRequest_1N_N_1SetMethod(JNIEnv* env,
                                                jobject obj,
                                                jlong self,
                                                jstring jmethod) {
  CefRefPtr<CefRequest> request = GetSelf(self);
  if (!request)
    return;
  request->SetMethod(GetJNIString(env, jmethod));
}

JNIEXPORT jobject JNICALL
Java_org_cef_network_CefRequest_1N_N_1GetPostData(JNIEnv* env,
                                                  jobject obj,
                                                  jlong self) {
  CefRefPtr<CefRequest> request = GetSelf(self);
  if (!request)
    return nullptr;
  CefRefPtr<CefPostData> postData = request->GetPostData();
  if (!postData.get())
    return nullptr;
  ScopedJNIPostData jpostData(env, postData);
  return jpostData.Release();
}

JNIEXPORT void JNICALL
Java_org_cef_network_CefRequest_1N_N_1SetPostData(JNIEnv* env,
                                                  jobject obj,
                                                  jlong self,
                                                  jobject jpostData) {
  CefRefPtr<CefRequest> request = GetSelf(self);
  if (!request)
    return;
  ScopedJNIPostData postDataObj(env);
  if (jpostData) {
    postDataObj.SetHandle(jpostData, false /* should_delete */);
  }
  request->SetPostData(postDataObj.GetCefObject());
}

JNIEXPORT jstring JNICALL
Java_org_cef_network_CefRequest_1N_N_1GetHeaderByName(JNIEnv* env,
                                                      jobject obj,
                                                      jlong self,
                                                      jstring jname) {
  CefRefPtr<CefRequest> request = GetSelf(self);
  if (!request)
    return nullptr;
  return NewJNIString(env, request->GetHeaderByName(GetJNIString(env, jname)));
}

JNIEXPORT void JNICALL
Java_org_cef_network_CefRequest_1N_N_1SetHeaderByName(JNIEnv* env,
                                                      jobject obj,
                                                      jlong self,
                                                      jstring jname,
                                                      jstring jvalue,
                                                      jboolean joverride) {
  CefRefPtr<CefRequest> request = GetSelf(self);
  if (!request)
    return;
  return request->SetHeaderByName(GetJNIString(env, jname),
                                  GetJNIString(env, jvalue),
                                  joverride != JNI_FALSE);
}

JNIEXPORT void JNICALL
Java_org_cef_network_CefRequest_1N_N_1GetHeaderMap(JNIEnv* env,
                                                   jobject obj,
                                                   jlong self,
                                                   jobject jheaderMap) {
  CefRefPtr<CefRequest> request = GetSelf(self);
  if (!request)
    return;
  CefRequest::HeaderMap headerMap;
  request->GetHeaderMap(headerMap);
  SetJNIStringMultiMap(env, jheaderMap, headerMap);
}

JNIEXPORT void JNICALL
Java_org_cef_network_CefRequest_1N_N_1SetHeaderMap(JNIEnv* env,
                                                   jobject obj,
                                                   jlong self,
                                                   jobject jheaderMap) {
  CefRefPtr<CefRequest> request = GetSelf(self);
  if (!request)
    return;
  CefRequest::HeaderMap headerMap;
  GetJNIStringMultiMap(env, jheaderMap, headerMap);
  request->SetHeaderMap(headerMap);
}

JNIEXPORT void JNICALL
Java_org_cef_network_CefRequest_1N_N_1Set(JNIEnv* env,
                                          jobject obj,
                                          jlong self,
                                          jstring jurl,
                                          jstring jmethod,
                                          jobject jpostData,
                                          jobject jheaderMap) {
  CefRefPtr<CefRequest> request = GetSelf(self);
  if (!request)
    return;

  CefRequest::HeaderMap headerMap;
  GetJNIStringMultiMap(env, jheaderMap, headerMap);
  request->SetHeaderMap(headerMap);

  ScopedJNIPostData postDataObj(env);
  if (jpostData) {
    postDataObj.SetHandle(jpostData, false /* should_delete */);
  }

  request->Set(GetJNIString(env, jurl), GetJNIString(env, jmethod),
               postDataObj.GetCefObject(), headerMap);
}

JNIEXPORT jint JNICALL
Java_org_cef_network_CefRequest_1N_N_1GetFlags(JNIEnv* env,
                                               jobject obj,
                                               jlong self) {
  CefRefPtr<CefRequest> request = GetSelf(self);
  if (!request)
    return 0;
  return request->GetFlags();
}

JNIEXPORT void JNICALL
Java_org_cef_network_CefRequest_1N_N_1SetFlags(JNIEnv* env,
                                               jobject obj,
                                               jlong self,
                                               jint jflags) {
  CefRefPtr<CefRequest> request = GetSelf(self);
  if (!request)
    return;
  request->SetFlags((int)jflags);
}

JNIEXPORT jstring JNICALL
Java_org_cef_network_CefRequest_1N_N_1GetFirstPartyForCookies(JNIEnv* env,
                                                              jobject obj,
                                                              jlong self) {
  CefRefPtr<CefRequest> request = GetSelf(self);
  if (!request)
    return nullptr;
  return NewJNIString(env, request->GetFirstPartyForCookies());
}

JNIEXPORT void JNICALL
Java_org_cef_network_CefRequest_1N_N_1SetFirstPartyForCookies(JNIEnv* env,
                                                              jobject obj,
                                                              jlong self,
                                                              jstring jcookie) {
  CefRefPtr<CefRequest> request = GetSelf(self);
  if (!request)
    return;
  request->SetFirstPartyForCookies(GetJNIString(env, jcookie));
}

JNIEXPORT jobject JNICALL
Java_org_cef_network_CefRequest_1N_N_1GetResourceType(JNIEnv* env,
                                                      jobject obj,
                                                      jlong self) {
  CefRefPtr<CefRequest> request = GetSelf(self);
  if (!request)
    return nullptr;

  jobject result = nullptr;
  CefRequest::ResourceType rt = request->GetResourceType();
  switch (rt) {
    JNI_CASE(env, "org/cef/network/CefRequest$ResourceType", RT_MAIN_FRAME,
             result);
    JNI_CASE(env, "org/cef/network/CefRequest$ResourceType", RT_SUB_FRAME,
             result);
    JNI_CASE(env, "org/cef/network/CefRequest$ResourceType", RT_STYLESHEET,
             result);
    JNI_CASE(env, "org/cef/network/CefRequest$ResourceType", RT_SCRIPT, result);
    JNI_CASE(env, "org/cef/network/CefRequest$ResourceType", RT_IMAGE, result);
    JNI_CASE(env, "org/cef/network/CefRequest$ResourceType", RT_FONT_RESOURCE,
             result);
    JNI_CASE(env, "org/cef/network/CefRequest$ResourceType", RT_SUB_RESOURCE,
             result);
    JNI_CASE(env, "org/cef/network/CefRequest$ResourceType", RT_OBJECT, result);
    JNI_CASE(env, "org/cef/network/CefRequest$ResourceType", RT_MEDIA, result);
    JNI_CASE(env, "org/cef/network/CefRequest$ResourceType", RT_WORKER, result);
    JNI_CASE(env, "org/cef/network/CefRequest$ResourceType", RT_SHARED_WORKER,
             result);
    JNI_CASE(env, "org/cef/network/CefRequest$ResourceType", RT_PREFETCH,
             result);
    JNI_CASE(env, "org/cef/network/CefRequest$ResourceType", RT_FAVICON,
             result);
    JNI_CASE(env, "org/cef/network/CefRequest$ResourceType", RT_XHR, result);
    JNI_CASE(env, "org/cef/network/CefRequest$ResourceType", RT_PING, result);
    JNI_CASE(env, "org/cef/network/CefRequest$ResourceType", RT_SERVICE_WORKER,
             result);
    JNI_CASE(env, "org/cef/network/CefRequest$ResourceType", RT_CSP_REPORT,
             result);
    JNI_CASE(env, "org/cef/network/CefRequest$ResourceType", RT_PLUGIN_RESOURCE,
             result);
    JNI_CASE(env, "org/cef/network/CefRequest$ResourceType",
             RT_NAVIGATION_PRELOAD_MAIN_FRAME, result);
    JNI_CASE(env, "org/cef/network/CefRequest$ResourceType",
             RT_NAVIGATION_PRELOAD_SUB_FRAME, result);
  }
  return result;
}

JNIEXPORT jobject JNICALL
Java_org_cef_network_CefRequest_1N_N_1GetTransitionType(JNIEnv* env,
                                                        jobject obj,
                                                        jlong self) {
  CefRefPtr<CefRequest> request = GetSelf(self);
  if (!request)
    return nullptr;
  ScopedJNITransitionType type(env, request->GetTransitionType());
  return type.Release();
}
