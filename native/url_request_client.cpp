// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "url_request_client.h"

#include "jni_util.h"
#include "util.h"

namespace {

const char kCefClassName[] = "CefURLRequestClient";

}  // namespace

URLRequestClient::URLRequestClient(JNIEnv* env,
                                   jobject jURLRequestClient,
                                   jobject jURLRequest)
    : client_handle_(env, jURLRequestClient),
      request_handle_(env, jURLRequest) {}

CefRefPtr<URLRequestClient> URLRequestClient::Create(JNIEnv* env,
                                                     jobject jURLRequestClient,
                                                     jobject jURLRequest) {
  CefRefPtr<URLRequestClient> client = GetCefFromJNIObject<URLRequestClient>(
      env, jURLRequestClient, kCefClassName);
  if (!client) {
    client = new URLRequestClient(env, jURLRequestClient, jURLRequest);
    SetCefForJNIObject(env, jURLRequestClient, client.get(), kCefClassName);
  }
  return client;
}

URLRequestClient::~URLRequestClient() {
  BEGIN_ENV(env)
  SetCefForJNIObject<URLRequestClient>(env, client_handle_, NULL,
                                       kCefClassName);
  END_ENV(env)
}

// TODO(jcef): Solve jurlReques instead of using NULL
void URLRequestClient::OnRequestComplete(CefRefPtr<CefURLRequest> request) {
  BEGIN_ENV(env)
  JNI_CALL_VOID_METHOD(env, client_handle_, "onRequestComplete",
                       "(Lorg/cef/network/CefURLRequest;)V",
                       request_handle_.get());
  END_ENV(env)
}

void URLRequestClient::OnUploadProgress(CefRefPtr<CefURLRequest> request,
                                        int64 current,
                                        int64 total) {
  BEGIN_ENV(env)
  JNI_CALL_VOID_METHOD(env, client_handle_, "onUploadProgress",
                       "(Lorg/cef/network/CefURLRequest;II)V",
                       request_handle_.get(), (jint)current, (jint)total);
  END_ENV(env)
}

void URLRequestClient::OnDownloadProgress(CefRefPtr<CefURLRequest> request,
                                          int64 current,
                                          int64 total) {
  BEGIN_ENV(env)
  JNI_CALL_VOID_METHOD(env, client_handle_, "onDownloadProgress",
                       "(Lorg/cef/network/CefURLRequest;II)V",
                       request_handle_.get(), (jint)current, (jint)total);
  END_ENV(env)
}

void URLRequestClient::OnDownloadData(CefRefPtr<CefURLRequest> request,
                                      const void* data,
                                      size_t data_length) {
  BEGIN_ENV(env)
  jbyteArray jbyteArray = env->NewByteArray((jsize)data_length);
  env->SetByteArrayRegion(jbyteArray, 0, (jsize)data_length,
                          (const jbyte*)data);

  JNI_CALL_VOID_METHOD(env, client_handle_, "onDownloadData",
                       "(Lorg/cef/network/CefURLRequest;[BI)V",
                       request_handle_.get(), jbyteArray, (jint)data_length);

  env->DeleteLocalRef(jbyteArray);
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

  ScopedJNIString jhost(env, host);
  ScopedJNIString jrealm(env, realm);
  ScopedJNIString jscheme(env, scheme);
  ScopedJNIAuthCallback jcallback(env, callback);

  JNI_CALL_METHOD(env, client_handle_, "getAuthCredentials",
                  "(ZLjava/lang/String;ILjava/lang/String;Ljava/lang/"
                  "String;Lorg/cef/callback/CefAuthCallback;)Z",
                  Boolean, jresult, (isProxy ? JNI_TRUE : JNI_FALSE),
                  jhost.get(), port, jrealm.get(), jscheme.get(),
                  jcallback.get());

  if (jresult == JNI_FALSE) {
    // If the Java method returns "false" the callback won't be used and
    // the reference can therefore be removed.
    jcallback.SetTemporary();
  }

  END_ENV(env)

  return (jresult != JNI_FALSE);
}
