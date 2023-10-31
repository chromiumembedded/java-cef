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
  ScopedJNIEnv env;
  if (!env)
    return;

  SetCefForJNIObject<URLRequestClient>(env, client_handle_, nullptr,
                                       kCefClassName);
}

// TODO(jcef): Solve jurlReques instead of using nullptr
void URLRequestClient::OnRequestComplete(CefRefPtr<CefURLRequest> request) {
  ScopedJNIEnv env;
  if (!env)
    return;

  JNI_CALL_VOID_METHOD(env, client_handle_, "onRequestComplete",
                       "(Lorg/cef/network/CefURLRequest;)V",
                       request_handle_.get());
}

void URLRequestClient::OnUploadProgress(CefRefPtr<CefURLRequest> request,
                                        int64_t current,
                                        int64_t total) {
  ScopedJNIEnv env;
  if (!env)
    return;

  JNI_CALL_VOID_METHOD(env, client_handle_, "onUploadProgress",
                       "(Lorg/cef/network/CefURLRequest;II)V",
                       request_handle_.get(), (jint)current, (jint)total);
}

void URLRequestClient::OnDownloadProgress(CefRefPtr<CefURLRequest> request,
                                          int64_t current,
                                          int64_t total) {
  ScopedJNIEnv env;
  if (!env)
    return;

  JNI_CALL_VOID_METHOD(env, client_handle_, "onDownloadProgress",
                       "(Lorg/cef/network/CefURLRequest;II)V",
                       request_handle_.get(), (jint)current, (jint)total);
}

void URLRequestClient::OnDownloadData(CefRefPtr<CefURLRequest> request,
                                      const void* data,
                                      size_t data_length) {
  ScopedJNIEnv env;
  if (!env)
    return;

  jbyteArray jbyteArray = env->NewByteArray((jsize)data_length);
  env->SetByteArrayRegion(jbyteArray, 0, (jsize)data_length,
                          (const jbyte*)data);

  JNI_CALL_VOID_METHOD(env, client_handle_, "onDownloadData",
                       "(Lorg/cef/network/CefURLRequest;[BI)V",
                       request_handle_.get(), jbyteArray, (jint)data_length);

  env->DeleteLocalRef(jbyteArray);
}

bool URLRequestClient::GetAuthCredentials(bool isProxy,
                                          const CefString& host,
                                          int port,
                                          const CefString& realm,
                                          const CefString& scheme,
                                          CefRefPtr<CefAuthCallback> callback) {
  jboolean jresult = JNI_FALSE;

  ScopedJNIEnv env;
  if (!env)
    return false;

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

  return (jresult != JNI_FALSE);
}
