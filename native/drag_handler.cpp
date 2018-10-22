// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "drag_handler.h"

#include "client_handler.h"
#include "jni_util.h"
#include "util.h"

DragHandler::DragHandler(JNIEnv* env, jobject handler) {
  jhandler_ = env->NewGlobalRef(handler);
}

DragHandler::~DragHandler() {
  JNIEnv* env = GetJNIEnv();
  env->DeleteGlobalRef(jhandler_);
}

bool DragHandler::OnDragEnter(CefRefPtr<CefBrowser> browser,
                              CefRefPtr<CefDragData> dragData,
                              CefDragHandler::DragOperationsMask mask) {
  JNIEnv* env = GetJNIEnv();
  if (!env)
    return false;

  jobject jdragdata = NewJNIObject(env, "org/cef/callback/CefDragData_N");
  if (!jdragdata)
    return false;
  SetCefForJNIObject(env, jdragdata, dragData.get(), "CefDragData");

  jboolean result = JNI_FALSE;
  JNI_CALL_METHOD(
      env, jhandler_, "onDragEnter",
      "(Lorg/cef/browser/CefBrowser;Lorg/cef/callback/CefDragData;I)Z", Boolean,
      result, GetJNIBrowser(browser), jdragdata, (jint)mask);

  // Remove native reference from java class
  SetCefForJNIObject<CefDragData>(env, jdragdata, NULL, "CefDragData");
  env->DeleteLocalRef(jdragdata);
  return (result != JNI_FALSE);
}
