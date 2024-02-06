// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "resource_handler.h"

#include "jni_util.h"
#include "util.h"

ResourceHandler::ResourceHandler(JNIEnv* env, jobject handler)
    : handle_(env, handler) {}

bool ResourceHandler::ProcessRequest(CefRefPtr<CefRequest> request,
                                     CefRefPtr<CefCallback> callback) {
  ScopedJNIEnv env;
  if (!env)
    return false;

  ScopedJNIRequest jrequest(env, request);
  jrequest.SetTemporary();
  ScopedJNICallback jcallback(env, callback);
  jboolean jresult = JNI_FALSE;

  JNI_CALL_METHOD(
      env, handle_, "processRequest",
      "(Lorg/cef/network/CefRequest;Lorg/cef/callback/CefCallback;)Z", Boolean,
      jresult, jrequest.get(), jcallback.get());

  if (jresult == JNI_FALSE) {
    // If the Java method returns "false" the callback won't be used and
    // the reference can therefore be removed.
    jcallback.SetTemporary();
  }

  return (jresult != JNI_FALSE);
}

void ResourceHandler::GetResponseHeaders(CefRefPtr<CefResponse> response,
                                         int64_t& response_length,
                                         CefString& redirectUrl) {
  ScopedJNIEnv env;
  if (!env)
    return;

  ScopedJNIResponse jresponse(env, response);
  jresponse.SetTemporary();
  ScopedJNIIntRef jresponseLength(env, (int)response_length);
  ScopedJNIStringRef jredirectUrl(env, redirectUrl);

  JNI_CALL_VOID_METHOD(env, handle_, "getResponseHeaders",
                       "(Lorg/cef/network/CefResponse;Lorg/cef/misc/"
                       "IntRef;Lorg/cef/misc/StringRef;)V",
                       jresponse.get(), jresponseLength.get(),
                       jredirectUrl.get());

  response_length = jresponseLength;
  redirectUrl = jredirectUrl;
}

bool ResourceHandler::ReadResponse(void* data_out,
                                   int bytes_to_read,
                                   int& bytes_read,
                                   CefRefPtr<CefCallback> callback) {
  ScopedJNIEnv env;
  if (!env)
    return false;

  ScopedJNIIntRef jbytesRead(env, bytes_read);
  jbyteArray jbytes = env->NewByteArray(bytes_to_read);
  ScopedJNICallback jcallback(env, callback);
  jboolean jresult = JNI_FALSE;

  JNI_CALL_METHOD(env, handle_, "readResponse",
                  "([BILorg/cef/misc/IntRef;Lorg/cef/callback/CefCallback;)Z",
                  Boolean, jresult, jbytes, bytes_to_read, jbytesRead.get(),
                  jcallback.get());

  bytes_read = jbytesRead;

  bool result = (jresult != JNI_FALSE);
  if (!result || bytes_read > 0) {
    // The callback won't be used and the reference can therefore be removed.
    jcallback.SetTemporary();
  }

  jbyte* jbyte = env->GetByteArrayElements(jbytes, nullptr);
  if (jbyte) {
    memmove(data_out, jbyte,
            (bytes_read < bytes_to_read ? bytes_read : bytes_to_read));
    env->ReleaseByteArrayElements(jbytes, jbyte, JNI_ABORT);
  }
  env->DeleteLocalRef(jbytes);

  return result;
}

void ResourceHandler::Cancel() {
  ScopedJNIEnv env;
  if (!env)
    return;
  JNI_CALL_VOID_METHOD(env, handle_, "cancel", "()V");
}
