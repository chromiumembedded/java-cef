// Copyright (c) 2017 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "CefFrame_N.h"

#include "include/base/cef_bind.h"
#include "include/cef_browser.h"
#include "include/cef_frame.h"

#include "critical_wait.h"
#include "jni_util.h"

JNIEXPORT void JNICALL
Java_org_cef_browser_CefFrame_1N_N_1CefFrame_1DTOR(JNIEnv* env, jobject obj) {
  SetCefForJNIObject<CefFrame>(env, obj, NULL, "CefFrame");
}

JNIEXPORT jlong JNICALL
Java_org_cef_browser_CefFrame_1N_N_1GetIdentifier(JNIEnv* env, jobject obj) {
  CefRefPtr<CefFrame> frame =
      GetCefFromJNIObject<CefFrame>(env, obj, "CefFrame");
  if (!frame)
    return (jlong)-1;
  return (jlong)frame->GetIdentifier();
}

JNIEXPORT jstring JNICALL
Java_org_cef_browser_CefFrame_1N_N_1GetURL(JNIEnv* env, jobject obj) {
  CefRefPtr<CefFrame> frame =
      GetCefFromJNIObject<CefFrame>(env, obj, "CefFrame");
  if (!frame)
    return NULL;
  return NewJNIString(env, frame->GetURL());
}

JNIEXPORT jstring JNICALL
Java_org_cef_browser_CefFrame_1N_N_1GetName(JNIEnv* env, jobject obj) {
  CefRefPtr<CefFrame> frame =
      GetCefFromJNIObject<CefFrame>(env, obj, "CefFrame");
  if (!frame)
    return NULL;
  return NewJNIString(env, frame->GetName());
}

JNIEXPORT jboolean JNICALL
Java_org_cef_browser_CefFrame_1N_N_1IsMain(JNIEnv* env, jobject obj) {
  CefRefPtr<CefFrame> frame =
      GetCefFromJNIObject<CefFrame>(env, obj, "CefFrame");
  if (!frame)
    return JNI_FALSE;
  return frame->IsMain() ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jboolean JNICALL
Java_org_cef_browser_CefFrame_1N_N_1IsValid(JNIEnv* env, jobject obj) {
  CefRefPtr<CefFrame> frame =
      GetCefFromJNIObject<CefFrame>(env, obj, "CefFrame");
  if (!frame)
    return JNI_FALSE;
  return frame->IsValid() ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jboolean JNICALL
Java_org_cef_browser_CefFrame_1N_N_1IsFocused(JNIEnv* env, jobject obj) {
  CefRefPtr<CefFrame> frame =
      GetCefFromJNIObject<CefFrame>(env, obj, "CefFrame");
  if (!frame)
    return JNI_FALSE;
  return frame->IsFocused() ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jobject JNICALL
Java_org_cef_browser_CefFrame_1N_N_1GetParent(JNIEnv* env, jobject obj) {
  CefRefPtr<CefFrame> frame =
      GetCefFromJNIObject<CefFrame>(env, obj, "CefFrame");
  if (!frame)
    return JNI_FALSE;
  return GetJNIFrame(env, frame->GetParent());
}

JNIEXPORT void JNICALL
Java_org_cef_browser_CefFrame_1N_N_1ExecuteJavaScript(JNIEnv* env,
                                                      jobject obj,
                                                      jstring code,
                                                      jstring url,
                                                      jint line) {
  CefRefPtr<CefFrame> frame =
      GetCefFromJNIObject<CefFrame>(env, obj, "CefFrame");
  if (!frame)
    return;
  frame->ExecuteJavaScript(GetJNIString(env, code), GetJNIString(env, url),
                           line);
}
