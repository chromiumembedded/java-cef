// Copyright (c) 2015 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "window_handler.h"

#include "jni_util.h"

WindowHandler::WindowHandler(JNIEnv* env, jobject handler)
    : handle_(env, handler) {}

bool WindowHandler::GetRect(CefRefPtr<CefBrowser> browser, CefRect& rect) {
  ScopedJNIEnv env;
  if (!env)
    return false;

  ScopedJNIBrowser jbrowser(env, browser);
  return GetRect(jbrowser, rect);
}

bool WindowHandler::GetRect(jobject browser, CefRect& rect) {
  ScopedJNIEnv env;
  if (!env)
    return false;

  ScopedJNIObjectResult jreturn(env);
  JNI_CALL_METHOD(env, handle_, "getRect",
                  "(Lorg/cef/browser/CefBrowser;)Ljava/awt/Rectangle;", Object,
                  jreturn, browser);
  if (jreturn) {
    rect = GetJNIRect(env, jreturn);
    return true;
  }
  return false;
}

void WindowHandler::OnMouseEvent(CefRefPtr<CefBrowser> browser,
                                 int mouseEvent,
                                 int absX,
                                 int absY,
                                 int modifier,
                                 int button) {
  ScopedJNIEnv env;
  if (!env)
    return;

  ScopedJNIBrowser jbrowser(env, browser);
  JNI_CALL_VOID_METHOD(env, handle_, "onMouseEvent",
                       "(Lorg/cef/browser/CefBrowser;IIIII)V", jbrowser.get(),
                       (jint)mouseEvent, (jint)absX, (jint)absY, (jint)modifier,
                       (jint)button);
}
