// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include <iostream>

#include "CefURLRequest_N.h"
#include "include/cef_request.h"
#include "include/cef_urlrequest.h"
#include "url_request_client.h"
#include "jni_util.h"

JNIEXPORT void JNICALL Java_org_cef_network_CefURLRequest_1N_N_1CefURLRequest_1CTOR
  (JNIEnv *env, jobject obj, jobject jrequest, jobject jRequestClient) {
  CefRefPtr<URLRequestClient> client = URLRequestClient::Create(env, jRequestClient, obj);
  CefRefPtr<CefRequest> request = GetCefFromJNIObject<CefRequest>(env, jrequest, "CefRequest");

  CefRefPtr<CefURLRequest> urlRequest = CefURLRequest::Create(request, client.get());
  if (!urlRequest.get())
    return;
  SetCefForJNIObject(env, obj, urlRequest.get(), "CefURLRequest");
}

JNIEXPORT jobject JNICALL Java_org_cef_network_CefURLRequest_1N_N_1GetRequestStatus
  (JNIEnv *env, jobject obj) {
  jobject result = GetJNIEnumValue(env,
                                   "org/cef/network/CefURLRequest$Status",
                                   "UR_UNKNOWN");
  CefRefPtr<CefURLRequest> urlRequest =
      GetCefFromJNIObject<CefURLRequest>(env, obj, "CefURLRequest");
  if (!urlRequest.get())
    return result;

  switch (urlRequest->GetRequestStatus()) {
    default:
    JNI_CASE(env, "org/cef/network/CefURLRequest$Status", UR_UNKNOWN, result);
    JNI_CASE(env, "org/cef/network/CefURLRequest$Status", UR_SUCCESS, result);
    JNI_CASE(env, "org/cef/network/CefURLRequest$Status", UR_IO_PENDING, result);
    JNI_CASE(env, "org/cef/network/CefURLRequest$Status", UR_CANCELED, result);
    JNI_CASE(env, "org/cef/network/CefURLRequest$Status", UR_FAILED, result);
  }
  return result;
}

JNIEXPORT jobject JNICALL Java_org_cef_network_CefURLRequest_1N_N_1GetRequestError
  (JNIEnv *env, jobject obj) {
  CefRefPtr<CefURLRequest> urlRequest =
      GetCefFromJNIObject<CefURLRequest>(env, obj, "CefURLRequest");
  if (!urlRequest.get())
    return NewJNIErrorCode(env, ERR_FAILED);
  return NewJNIErrorCode(env, urlRequest->GetRequestError());
}

JNIEXPORT jobject JNICALL Java_org_cef_network_CefURLRequest_1N_N_1GetResponse
  (JNIEnv *env, jobject obj) {
  CefRefPtr<CefURLRequest> urlRequest =
      GetCefFromJNIObject<CefURLRequest>(env, obj, "CefURLRequest");
  if (!urlRequest.get())
    return NULL;

  CefRefPtr<CefResponse> response = urlRequest->GetResponse();
  if (!response.get())
    return NULL;

  jobject jresponse = NewJNIObject(env, "org/cef/network/CefResponse_N");
  SetCefForJNIObject<CefResponse>(env, jresponse, response, "CefResponse");
  return jresponse;
}

JNIEXPORT void JNICALL Java_org_cef_network_CefURLRequest_1N_N_1Cancel
  (JNIEnv *env, jobject obj) {
  CefRefPtr<CefURLRequest> urlRequest =
      GetCefFromJNIObject<CefURLRequest>(env, obj, "CefURLRequest");
  if (!urlRequest.get())
    return;

  CefRefPtr<URLRequestClient> client = (URLRequestClient*)urlRequest->GetClient().get();
  if (!client.get())
    return;

  urlRequest->Cancel();
}

JNIEXPORT void JNICALL Java_org_cef_network_CefURLRequest_1N_N_1CefURLRequest_1DTOR
  (JNIEnv *env, jobject obj) {
  SetCefForJNIObject<CefPostData>(env, obj, NULL, "CefURLRequest");
}
