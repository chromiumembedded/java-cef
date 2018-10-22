// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "resource_handler.h"

#include "jni_util.h"
#include "util.h"

ResourceHandler::ResourceHandler(JNIEnv* env, jobject handler) {
  jhandler_ = env->NewGlobalRef(handler);
}

ResourceHandler::~ResourceHandler() {
  JNIEnv* env = GetJNIEnv();
  env->DeleteGlobalRef(jhandler_);
}

bool ResourceHandler::ProcessRequest(CefRefPtr<CefRequest> request,
                                     CefRefPtr<CefCallback> callback) {
  JNIEnv* env = GetJNIEnv();
  if (!env)
    return false;

  jobject jrequest = NewJNIObject(env, "org/cef/network/CefRequest_N");
  if (!jrequest)
    return false;
  jobject jcallback = NewJNIObject(env, "org/cef/callback/CefCallback_N");
  if (!jcallback) {
    env->DeleteLocalRef(jrequest);
    return false;
  }
  SetCefForJNIObject(env, jrequest, request.get(), "CefRequest");
  SetCefForJNIObject(env, jcallback, callback.get(), "CefCallback");

  jboolean result = JNI_FALSE;
  JNI_CALL_METHOD(
      env, jhandler_, "processRequest",
      "(Lorg/cef/network/CefRequest;Lorg/cef/callback/CefCallback;)Z", Boolean,
      result, jrequest, jcallback);
  if (result == JNI_FALSE) {
    SetCefForJNIObject<CefRequest>(env, jrequest, NULL, "CefRequest");
    SetCefForJNIObject<CefCallback>(env, jcallback, NULL, "CefCallback");
  }
  env->DeleteLocalRef(jcallback);
  env->DeleteLocalRef(jrequest);
  return (result != JNI_FALSE);
}

void ResourceHandler::GetResponseHeaders(CefRefPtr<CefResponse> response,
                                         int64& response_length,
                                         CefString& redirectUrl) {
  JNIEnv* env = GetJNIEnv();
  if (!env)
    return;

  jobject jresponse = NewJNIObject(env, "org/cef/network/CefResponse_N");
  if (!jresponse)
    return;
  jobject jintRef = NewJNIIntRef(env, response_length);
  if (!jintRef) {
    env->DeleteLocalRef(jresponse);
    return;
  }
  jobject jstringRef = NewJNIStringRef(env, redirectUrl);
  if (!jstringRef) {
    env->DeleteLocalRef(jintRef);
    env->DeleteLocalRef(jresponse);
    return;
  }

  SetCefForJNIObject(env, jresponse, response.get(), "CefResponse");

  JNI_CALL_VOID_METHOD(env, jhandler_, "getResponseHeaders",
                       "(Lorg/cef/network/CefResponse;Lorg/cef/misc/"
                       "IntRef;Lorg/cef/misc/StringRef;)V",
                       jresponse, jintRef, jstringRef);
  response_length = GetJNIIntRef(env, jintRef);
  redirectUrl = GetJNIStringRef(env, jstringRef);
  SetCefForJNIObject<CefResponse>(env, jresponse, NULL, "CefResponse");
  env->DeleteLocalRef(jstringRef);
  env->DeleteLocalRef(jintRef);
  env->DeleteLocalRef(jresponse);
  return;
}

bool ResourceHandler::ReadResponse(void* data_out,
                                   int bytes_to_read,
                                   int& bytes_read,
                                   CefRefPtr<CefCallback> callback) {
  JNIEnv* env = GetJNIEnv();
  if (!env)
    return false;

  jobject jintRef = NewJNIIntRef(env, bytes_read);
  if (!jintRef)
    return false;
  jobject jcallback = NewJNIObject(env, "org/cef/callback/CefCallback_N");
  if (!jcallback) {
    env->DeleteLocalRef(jintRef);
    return false;
  }
  SetCefForJNIObject(env, jcallback, callback.get(), "CefCallback");

  jbyteArray jbytes = env->NewByteArray(bytes_to_read);
  jboolean jresult = JNI_FALSE;

  JNI_CALL_METHOD(env, jhandler_, "readResponse",
                  "([BILorg/cef/misc/IntRef;Lorg/cef/callback/CefCallback;)Z",
                  Boolean, jresult, jbytes, bytes_to_read, jintRef, jcallback);
  bool result = (jresult != JNI_FALSE);
  bytes_read = GetJNIIntRef(env, jintRef);
  if (!result || bytes_read > 0) {
    // Only if bytes are available at a later time, CefCallback will be called
    // This is true if result = true and bytes_read == 0)
    // In all other cases we have to release the reference to the native
    // CefCallback object.
    SetCefForJNIObject<CefCallback>(env, jcallback, NULL, "CefCallback");
  }
  jbyte* jbyte = env->GetByteArrayElements(jbytes, NULL);
  memmove(data_out, jbyte,
          (bytes_read < bytes_to_read ? bytes_read : bytes_to_read));
  env->ReleaseByteArrayElements(jbytes, jbyte, JNI_ABORT);
  env->DeleteLocalRef(jbytes);
  env->DeleteLocalRef(jintRef);
  env->DeleteLocalRef(jcallback);
  return result;
}

bool ResourceHandler::CanGetCookie(const CefCookie& cookie) {
  JNIEnv* env = GetJNIEnv();
  if (!env)
    return false;

  jboolean result = JNI_FALSE;
  JNI_CALL_METHOD(env, jhandler_, "canGetCookie",
                  "(Lorg/cef/network/CefCookie;)Z", Boolean, result,
                  NewJNICookie(env, cookie));
  return (result != JNI_FALSE);
}

bool ResourceHandler::CanSetCookie(const CefCookie& cookie) {
  JNIEnv* env = GetJNIEnv();
  if (!env)
    return false;

  jboolean result = JNI_FALSE;
  JNI_CALL_METHOD(env, jhandler_, "canSetCookie",
                  "(Lorg/cef/network/CefCookie;)Z", Boolean, result,
                  NewJNICookie(env, cookie));
  return (result != JNI_FALSE);
}

void ResourceHandler::Cancel() {
  JNIEnv* env = GetJNIEnv();
  if (!env)
    return;
  JNI_CALL_VOID_METHOD(env, jhandler_, "cancel", "()V");
}
