// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "CefQueryCallback_N.h"
#include "include/wrapper/cef_message_router.h"
#include "jni_scoped_helpers.h"
#include "jni_util.h"

namespace {

using CefQueryCallback = CefMessageRouterBrowserSide::Callback;

CefRefPtr<CefQueryCallback> GetSelf(jlong self) {
  return reinterpret_cast<CefQueryCallback*>(self);
}

void ClearSelf(JNIEnv* env, jobject obj) {
  // Clear the reference added in ClientHandler::OnQuery.
  SetCefForJNIObject<CefQueryCallback>(env, obj, nullptr, "CefQueryCallback");
}

}  // namespace

JNIEXPORT void JNICALL
Java_org_cef_callback_CefQueryCallback_1N_N_1Success(JNIEnv* env,
                                                     jobject obj,
                                                     jlong self,
                                                     jstring response,
                                                     jboolean persistent) {
  CefRefPtr<CefQueryCallback> callback = GetSelf(self);
  if (!callback)
    return;
  callback->Success(GetJNIString(env, response));
  if (persistent)
    return;
  ClearSelf(env, obj);
}

JNIEXPORT void JNICALL
Java_org_cef_callback_CefQueryCallback_1N_N_1SuccessBinary(
    JNIEnv* env,
    jobject obj,
    jlong self,
    jobject response,
    jboolean persistent) {
  CefRefPtr<CefQueryCallback> callback = GetSelf(self);
  if (!callback)
    return;
  jboolean isDirect = JNI_TRUE;
  if (response)
    JNI_CALL_BOOLEAN_METHOD(isDirect, env, response, "isDirect", "()Z");
  if (isDirect != JNI_FALSE) {
    callback->Success(GetJNIByteBufferData(env, response),
                      GetJNIByteBufferLength(env, response));
  } else {
    jobject responseArrayObj = nullptr;
    jint responseOffset = 0;
    jint responseLength = 0;
    JNI_CALL_METHOD(env, response, "array", "()[B", Object, responseArrayObj);
    JNI_CALL_METHOD(env, response, "arrayOffset", "()I", Int, responseOffset);
    JNI_CALL_METHOD(env, response, "capacity", "()I", Int, responseLength);
    jbyteArray responseArray = static_cast<jbyteArray>(responseArrayObj);
    jsize responseArrayLength = env->GetArrayLength(responseArray);
    if (responseLength >= 0 && responseLength <= responseArrayLength &&
        responseOffset >= 0 && responseOffset < responseLength) {
      // isCopy is ignored
      jboolean isCopy = JNI_FALSE;
      jbyte *responseBytes = env->GetByteArrayElements(responseArray, &isCopy);
      callback->Success(static_cast<void*>(responseBytes + responseOffset),
                        static_cast<size_t>(responseLength));
      // JNI_ABORT: no need to copy back the data
      env->ReleaseByteArrayElements(responseArray, responseBytes, JNI_ABORT);
    }
  }
  if (persistent)
    return;
  ClearSelf(env, obj);
}

JNIEXPORT void JNICALL
Java_org_cef_callback_CefQueryCallback_1N_N_1Failure(JNIEnv* env,
                                                     jobject obj,
                                                     jlong self,
                                                     jint error_code,
                                                     jstring error_message) {
  CefRefPtr<CefQueryCallback> callback = GetSelf(self);
  if (!callback)
    return;
  callback->Failure(error_code, GetJNIString(env, error_message));
  ClearSelf(env, obj);
}
