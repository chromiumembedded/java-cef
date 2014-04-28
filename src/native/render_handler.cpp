// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "render_handler.h"
#include "client_handler.h"

#include "jni_util.h"
#include "util.h"

RenderHandler::RenderHandler(JNIEnv* env, jobject handler) {
  jhandler_ = env->NewGlobalRef(handler);
}

RenderHandler::~RenderHandler() {
  JNIEnv* env = GetJNIEnv();
  env->DeleteGlobalRef(jhandler_);
}

bool RenderHandler::GetRootScreenRect(CefRefPtr<CefBrowser> browser,
                                      CefRect& rect) {
  return GetViewRect(browser, rect);
}

bool RenderHandler::GetViewRect(CefRefPtr<CefBrowser> browser, CefRect& rect) {
  return GetViewRect(GetJNIBrowser(browser), rect);
}

bool RenderHandler::GetScreenPoint(CefRefPtr<CefBrowser> browser,
                            int viewX,
                            int viewY,
                            int& screenX,
                            int& screenY) {
  return GetScreenPoint(GetJNIBrowser(browser), viewX, viewY, screenX, screenY);
}

void RenderHandler::OnPopupShow(CefRefPtr<CefBrowser> browser,
                                bool show) {
  JNIEnv* env = GetJNIEnv();
  if (!env)
  	return;
  JNI_CALL_VOID_METHOD(env, jhandler_, 
                       "onPopupShow", 
                       "(Lorg/cef/browser/CefBrowser;Z)V", 
                       GetJNIBrowser(browser),
                       (jboolean)show);
}

void RenderHandler::OnPopupSize(CefRefPtr<CefBrowser> browser,
                                const CefRect& rect) {
  JNIEnv* env = GetJNIEnv();
  if (!env)
  	return;
  jobject rect_obj = NewJNIRect(env, rect);
  if (!rect_obj)
    return;
  JNI_CALL_VOID_METHOD(env, jhandler_, 
                       "onPopupSize", 
                       "(Lorg/cef/browser/CefBrowser;Ljava/awt/Rectangle;)V", 
                       GetJNIBrowser(browser),
                       rect_obj);
  env->DeleteLocalRef(rect_obj);
}

void RenderHandler::OnPaint(CefRefPtr<CefBrowser> browser,
                            PaintElementType type,
                            const RectList& dirtyRects,
                            const void* buffer,
                            int width, int height) {
  JNIEnv* env = GetJNIEnv();
  if (!env)
  	return;
  jobjectArray rect_array = NewJNIRectArray(env, dirtyRects);
  jobject direct_buffer =
      env->NewDirectByteBuffer(const_cast<void*>(buffer), width * height * 4);
  jboolean jtype = type == PET_VIEW ? JNI_FALSE : JNI_TRUE;
  JNI_CALL_VOID_METHOD(env, jhandler_, 
                       "onPaint", 
                       "(Lorg/cef/browser/CefBrowser;Z[Ljava/awt/Rectangle;Ljava/nio/ByteBuffer;II)V", 
                       GetJNIBrowser(browser),
                       jtype,
                       rect_array,
                       direct_buffer,
                       width,
                       height);
  env->DeleteLocalRef(rect_array);
  env->DeleteLocalRef(direct_buffer);
}

void RenderHandler::OnCursorChange(CefRefPtr<CefBrowser> browser,
                            CefCursorHandle cursor) {
  JNIEnv* env = GetJNIEnv();
  if (!env)
  	return;
  const int cursorId = NativeGetCursorId(cursor);
  JNI_CALL_VOID_METHOD(env, jhandler_, 
                       "onCursorChange", 
                       "(Lorg/cef/browser/CefBrowser;I)V", 
                       GetJNIBrowser(browser),
                       cursorId);
}


bool RenderHandler::GetViewRect(jobject browser, CefRect& rect) {
  JNIEnv* env = GetJNIEnv();
  if (!env)
  	return false;
  jobject jreturn = NULL;
  JNI_CALL_METHOD(env, jhandler_, 
                  "getViewRect", 
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

bool RenderHandler::GetScreenPoint(jobject browser,
                                   int viewX,
                                   int viewY,
                                   int& screenX,
                                   int& screenY) {
  JNIEnv* env = GetJNIEnv();
  if (!env)
  	return false;

  jobject jreturn = NULL;
  jobject point_obj = NewJNIPoint(env, viewX, viewY);
  if (!point_obj)
    return false;

  JNI_CALL_METHOD(env, jhandler_, 
                  "getScreenPoint", 
                  "(Lorg/cef/browser/CefBrowser;Ljava/awt/Point;)Ljava/awt/Point;",
                  Object,
                  jreturn, 
                  browser,
                  point_obj);
  if (jreturn) {
    GetJNIPoint(env, jreturn, &screenX, &screenY);
    env->DeleteLocalRef(jreturn);
    return true;
  }
  return false;
}
