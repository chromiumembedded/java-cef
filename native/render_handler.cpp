// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "render_handler.h"

#include "client_handler.h"
#include "jni_util.h"
#include "util.h"

namespace {

int GetCursorId(cef_cursor_type_t type) {
  JNIEnv* env = GetJNIEnv();
  if (!env)
    return 0;

  jclass cls = FindClass(env, "java/awt/Cursor");
  if (!cls)
    return 0;

  JNI_STATIC_DEFINE_INT_RV(env, cls, CROSSHAIR_CURSOR, 0);
  JNI_STATIC_DEFINE_INT_RV(env, cls, DEFAULT_CURSOR, 0);
  JNI_STATIC_DEFINE_INT_RV(env, cls, E_RESIZE_CURSOR, 0);
  JNI_STATIC_DEFINE_INT_RV(env, cls, HAND_CURSOR, 0);
  JNI_STATIC_DEFINE_INT_RV(env, cls, MOVE_CURSOR, 0);
  JNI_STATIC_DEFINE_INT_RV(env, cls, N_RESIZE_CURSOR, 0);
  JNI_STATIC_DEFINE_INT_RV(env, cls, NE_RESIZE_CURSOR, 0);
  JNI_STATIC_DEFINE_INT_RV(env, cls, NW_RESIZE_CURSOR, 0);
  JNI_STATIC_DEFINE_INT_RV(env, cls, S_RESIZE_CURSOR, 0);
  JNI_STATIC_DEFINE_INT_RV(env, cls, SE_RESIZE_CURSOR, 0);
  JNI_STATIC_DEFINE_INT_RV(env, cls, SW_RESIZE_CURSOR, 0);
  JNI_STATIC_DEFINE_INT_RV(env, cls, TEXT_CURSOR, 0);
  JNI_STATIC_DEFINE_INT_RV(env, cls, W_RESIZE_CURSOR, 0);
  JNI_STATIC_DEFINE_INT_RV(env, cls, WAIT_CURSOR, 0);

  switch (type) {
    case CT_CROSS:
      return JNI_STATIC(CROSSHAIR_CURSOR);
    case CT_HAND:
      return JNI_STATIC(HAND_CURSOR);
    case CT_IBEAM:
      return JNI_STATIC(TEXT_CURSOR);
    case CT_WAIT:
      return JNI_STATIC(WAIT_CURSOR);
    case CT_EASTRESIZE:
      return JNI_STATIC(E_RESIZE_CURSOR);
    case CT_NORTHRESIZE:
      return JNI_STATIC(N_RESIZE_CURSOR);
    case CT_NORTHEASTRESIZE:
      return JNI_STATIC(NE_RESIZE_CURSOR);
    case CT_NORTHWESTRESIZE:
      return JNI_STATIC(NW_RESIZE_CURSOR);
    case CT_SOUTHRESIZE:
      return JNI_STATIC(S_RESIZE_CURSOR);
    case CT_SOUTHEASTRESIZE:
      return JNI_STATIC(SE_RESIZE_CURSOR);
    case CT_SOUTHWESTRESIZE:
      return JNI_STATIC(SW_RESIZE_CURSOR);
    case CT_WESTRESIZE:
      return JNI_STATIC(W_RESIZE_CURSOR);
    case CT_MOVE:
      return JNI_STATIC(MOVE_CURSOR);
    default:
      return JNI_STATIC(DEFAULT_CURSOR);
  }
}

}  // namespace

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

void RenderHandler::OnPopupShow(CefRefPtr<CefBrowser> browser, bool show) {
  JNIEnv* env = GetJNIEnv();
  if (!env)
    return;
  JNI_CALL_VOID_METHOD(env, jhandler_, "onPopupShow",
                       "(Lorg/cef/browser/CefBrowser;Z)V",
                       GetJNIBrowser(browser), (jboolean)show);
}

void RenderHandler::OnPopupSize(CefRefPtr<CefBrowser> browser,
                                const CefRect& rect) {
  JNIEnv* env = GetJNIEnv();
  if (!env)
    return;
  jobject rect_obj = NewJNIRect(env, rect);
  if (!rect_obj)
    return;
  JNI_CALL_VOID_METHOD(env, jhandler_, "onPopupSize",
                       "(Lorg/cef/browser/CefBrowser;Ljava/awt/Rectangle;)V",
                       GetJNIBrowser(browser), rect_obj);
  env->DeleteLocalRef(rect_obj);
}

void RenderHandler::OnPaint(CefRefPtr<CefBrowser> browser,
                            PaintElementType type,
                            const RectList& dirtyRects,
                            const void* buffer,
                            int width,
                            int height) {
  JNIEnv* env = GetJNIEnv();
  if (!env)
    return;
  jobjectArray rect_array = NewJNIRectArray(env, dirtyRects);
  jobject direct_buffer =
      env->NewDirectByteBuffer(const_cast<void*>(buffer), width * height * 4);
  jboolean jtype = type == PET_VIEW ? JNI_FALSE : JNI_TRUE;
  JNI_CALL_VOID_METHOD(env, jhandler_, "onPaint",
                       "(Lorg/cef/browser/CefBrowser;Z[Ljava/awt/"
                       "Rectangle;Ljava/nio/ByteBuffer;II)V",
                       GetJNIBrowser(browser), jtype, rect_array, direct_buffer,
                       width, height);
  env->DeleteLocalRef(rect_array);
  env->DeleteLocalRef(direct_buffer);
}

// TODO(JCEF): Expose all parameters.
void RenderHandler::OnCursorChange(CefRefPtr<CefBrowser> browser,
                                   CefCursorHandle cursor,
                                   CursorType type,
                                   const CefCursorInfo& custom_cursor_info) {
  JNIEnv* env = GetJNIEnv();
  if (!env)
    return;
  const int cursorId = GetCursorId(type);
  JNI_CALL_VOID_METHOD(env, jhandler_, "onCursorChange",
                       "(Lorg/cef/browser/CefBrowser;I)V",
                       GetJNIBrowser(browser), cursorId);
}

bool RenderHandler::StartDragging(CefRefPtr<CefBrowser> browser,
                                  CefRefPtr<CefDragData> drag_data,
                                  DragOperationsMask allowed_ops,
                                  int x,
                                  int y) {
  JNIEnv* env = GetJNIEnv();
  if (!env)
    return false;

  jobject jdragdata = NewJNIObject(env, "org/cef/callback/CefDragData_N");
  if (!jdragdata)
    return false;

  SetCefForJNIObject(env, jdragdata, drag_data.get(), "CefDragData");

  jboolean jresult = JNI_FALSE;
  JNI_CALL_METHOD(
      env, jhandler_, "startDragging",
      "(Lorg/cef/browser/CefBrowser;Lorg/cef/callback/CefDragData;III)Z",
      Boolean, jresult, GetJNIBrowser(browser), jdragdata, (jint)allowed_ops,
      (jint)x, (jint)y);

  bool result = (jresult != JNI_FALSE);
  if (!result) {
    // if result == false the native reference must be deleted
    SetCefForJNIObject<CefDragData>(env, jdragdata, NULL, "CefDragData");
  }
  env->DeleteLocalRef(jdragdata);
  return result;
}

void RenderHandler::UpdateDragCursor(CefRefPtr<CefBrowser> browser,
                                     DragOperation operation) {
  JNIEnv* env = GetJNIEnv();
  if (!env)
    return;

  JNI_CALL_VOID_METHOD(env, jhandler_, "updateDragCursor",
                       "(Lorg/cef/browser/CefBrowser;I)V",
                       GetJNIBrowser(browser), (jint)operation);
}

bool RenderHandler::GetViewRect(jobject browser, CefRect& rect) {
  JNIEnv* env = GetJNIEnv();
  if (!env)
    return false;
  jobject jreturn = NULL;
  JNI_CALL_METHOD(env, jhandler_, "getViewRect",
                  "(Lorg/cef/browser/CefBrowser;)Ljava/awt/Rectangle;", Object,
                  jreturn, browser);
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

  JNI_CALL_METHOD(
      env, jhandler_, "getScreenPoint",
      "(Lorg/cef/browser/CefBrowser;Ljava/awt/Point;)Ljava/awt/Point;", Object,
      jreturn, browser, point_obj);
  if (jreturn) {
    GetJNIPoint(env, jreturn, &screenX, &screenY);
    env->DeleteLocalRef(jreturn);
    return true;
  }
  return false;
}
