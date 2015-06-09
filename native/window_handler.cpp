// Copyright (c) 2015 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "window_handler.h"

#include "jni_util.h"
#include "util.h"

WindowHandler::WindowHandler(JNIEnv* env, jobject handler) {
  jhandler_ = env->NewGlobalRef(handler);
}

WindowHandler::~WindowHandler() {
  JNIEnv* env = GetJNIEnv();
  env->DeleteGlobalRef(jhandler_);
}

bool WindowHandler::GetRect(CefRefPtr<CefBrowser> browser, CefRect& rect) {
  return GetRect(GetJNIBrowser(browser), rect);
}

bool WindowHandler::GetRect(jobject browser, CefRect& rect) {
  JNIEnv* env = GetJNIEnv();
  if (!env)
    return false;
  jobject jreturn = NULL;
  JNI_CALL_METHOD(env, jhandler_, 
                  "getRect", 
                  "(Lorg/cef/browser/CefBrowser;)Ljava/awt/Rectangle;",
                  Object,
                  jreturn, 
                  browser);
  if (jreturn) {
    rect = GetJNIRect(env, jreturn);
    env->DeleteLocalRef(jreturn);
    return true;
  }
  return false;
}

void WindowHandler::OnMouseEvent(CefRefPtr<CefBrowser> browser, int mouseEvent,
    int absX, int absY, int modifier, int button) {
  JNIEnv* env = GetJNIEnv();
  if (!env)
    return;
  JNI_CALL_VOID_METHOD(env, jhandler_,
                       "onMouseEvent",
                       "(Lorg/cef/browser/CefBrowser;IIIII)V",
                       GetJNIBrowser(browser),
                       (jint)mouseEvent,
                       (jint)absX,
                       (jint)absY,
                       (jint)modifier,
                       (jint)button);
}
