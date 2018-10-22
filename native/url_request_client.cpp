// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "url_request_client.h"

#include "jni_util.h"
#include "util.h"

URLRequestClient::URLRequestClient(JNIEnv* env,
                                   jobject jURLRequestClient,
                                   jobject jURLRequest) {
  jURLRequestClient_ = env->NewGlobalRef(jURLRequestClient);
  jURLRequest_ = env->NewGlobalRef(jURLRequest);
}

CefRefPtr<URLRequestClient> URLRequestClient::Create(JNIEnv* env,
                                                     jobject jURLRequestClient,
                                                     jobject jURLRequest) {
  CefRefPtr<URLRequestClient> client = NULL;
  client = GetCefFromJNIObject<URLRequestClient>(env, jURLRequestClient,
                                                 "CefURLRequestClient");
  if (!client.get()) {
    client = new URLRequestClient(env, jURLRequestClient, jURLRequest);
    SetCefForJNIObject(env, jURLRequestClient, client.get(),
                       "CefURLRequestClient");
  }
  return client;
}

URLRequestClient::~URLRequestClient() {
  BEGIN_ENV(env)
  SetCefForJNIObject<URLRequestClient>(env, jURLRequestClient_, NULL,
                                       "CefURLRequestClient");
  env->DeleteGlobalRef(jURLRequestClient_);
  env->DeleteGlobalRef(jURLRequest_);
  END_ENV(env)
}

// TODO(jcef): Solve jurlReques instead of using NULL
void URLRequestClient::OnRequestComplete(CefRefPtr<CefURLRequest> request) {
  BEGIN_ENV(env)
  JNI_CALL_VOID_METHOD(env, jURLRequestClient_, "onRequestComplete",
                       "(Lorg/cef/network/CefURLRequest;)V", jURLRequest_);
  END_ENV(env)
}

void URLRequestClient::OnUploadProgress(CefRefPtr<CefURLRequest> request,
                                        int64 current,
                                        int64 total) {
  BEGIN_ENV(env)
  JNI_CALL_VOID_METHOD(env, jURLRequestClient_, "onUploadProgress",
                       "(Lorg/cef/network/CefURLRequest;II)V", jURLRequest_,
                       (jint)current, (jint)total);
  END_ENV(env)
}

void URLRequestClient::OnDownloadProgress(CefRefPtr<CefURLRequest> request,
                                          int64 current,
                                          int64 total) {
  BEGIN_ENV(env)
  JNI_CALL_VOID_METHOD(env, jURLRequestClient_, "onDownloadProgress",
                       "(Lorg/cef/network/CefURLRequest;II)V", jURLRequest_,
                       (jint)current, (jint)total);
  END_ENV(env)
}

void URLRequestClient::OnDownloadData(CefRefPtr<CefURLRequest> request,
                                      const void* data,
                                      size_t data_length) {
  BEGIN_ENV(env)
  jbyteArray jbyteArray = env->NewByteArray((jsize)data_length);
  env->SetByteArrayRegion(jbyteArray, 0, (jsize)data_length,
                          (const jbyte*)data);

  JNI_CALL_VOID_METHOD(env, jURLRequestClient_, "onDownloadData",
                       "(Lorg/cef/network/CefURLRequest;[BI)V", jURLRequest_,
                       jbyteArray, (jint)data_length);
  END_ENV(env)
}

bool URLRequestClient::GetAuthCredentials(bool isProxy,
                                          const CefString& host,
                                          int port,
                                          const CefString& realm,
                                          const CefString& scheme,
                                          CefRefPtr<CefAuthCallback> callback) {
  jboolean jresult = JNI_FALSE;
  BEGIN_ENV(env)

  jobject jcallback = NewJNIObject(env, "org/cef/callback/CefAuthCallback_N");
  SetCefForJNIObject<CefAuthCallback>(env, jcallback, callback,
                                      "CefAuthCallback");

  jstring jhost = NewJNIString(env, host);
  jstring jrealm = NewJNIString(env, realm);
  jstring jscheme = NewJNIString(env, scheme);
  JNI_CALL_METHOD(env, jURLRequestClient_, "getAuthCredentials",
                  "(ZLjava/lang/String;ILjava/lang/String;Ljava/lang/"
                  "String;Lorg/cef/callback/CefAuthCallback;)Z",
                  Boolean, jresult, (isProxy ? JNI_TRUE : JNI_FALSE), jhost,
                  port, jrealm, jscheme, jcallback);
  env->DeleteLocalRef(jhost);
  env->DeleteLocalRef(jrealm);
  env->DeleteLocalRef(jscheme);

  if (jresult == JNI_FALSE)
    SetCefForJNIObject<CefAuthCallback>(env, jcallback, NULL,
                                        "CefAuthCallback");
  env->DeleteLocalRef(jcallback);

  END_ENV(env)

  return (jresult != JNI_FALSE);
}
