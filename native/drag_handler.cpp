// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "drag_handler.h"

#include "jni_util.h"

DragHandler::DragHandler(JNIEnv* env, jobject handler)
    : handle_(env, handler) {}

bool DragHandler::OnDragEnter(CefRefPtr<CefBrowser> browser,
                              CefRefPtr<CefDragData> dragData,
                              CefDragHandler::DragOperationsMask mask) {
  ScopedJNIEnv env;
  if (!env)
    return false;

  ScopedJNIBrowser jbrowser(env, browser);
  ScopedJNIDragData jdragData(env, dragData);
  jdragData.SetTemporary();
  jboolean result = JNI_FALSE;

  JNI_CALL_METHOD(
      env, handle_, "onDragEnter",
      "(Lorg/cef/browser/CefBrowser;Lorg/cef/callback/CefDragData;I)Z", Boolean,
      result, jbrowser.get(), jdragData.get(), (jint)mask);

  return (result != JNI_FALSE);
}
