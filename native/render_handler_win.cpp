// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "render_handler.h"
#include "jni_util.h"

int RenderHandler::NativeGetCursorId(CefCursorHandle cursor) {
  JNIEnv* env = GetJNIEnv();
  if(!env)
    return 0;

  jclass cls = env->FindClass("java/awt/Cursor");
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

  static HCURSOR kCursorArrow = LoadCursor(NULL, IDC_ARROW);
  static HCURSOR kCursorIBeam = LoadCursor(NULL, IDC_IBEAM);
  static HCURSOR kCursorWait = LoadCursor(NULL, IDC_WAIT);
  static HCURSOR kCursorCross = LoadCursor(NULL, IDC_CROSS);
  static HCURSOR kCursorUpArrow = LoadCursor(NULL, IDC_UPARROW);
  static HCURSOR kCursorSize = LoadCursor(NULL, IDC_SIZE);
  static HCURSOR kCursorIcon = LoadCursor(NULL, IDC_ICON);
  static HCURSOR kCursorSizeNWSE = LoadCursor(NULL, IDC_SIZENWSE);
  static HCURSOR kCursorSizeNESW = LoadCursor(NULL, IDC_SIZENESW);
  static HCURSOR kCursorSizeWE = LoadCursor(NULL, IDC_SIZEWE);
  static HCURSOR kCursorSizeNS = LoadCursor(NULL, IDC_SIZENS);
  static HCURSOR kCursorSizeAll = LoadCursor(NULL, IDC_SIZEALL);
  static HCURSOR kCursorNo = LoadCursor(NULL, IDC_NO);
  static HCURSOR kCursorHand = LoadCursor(NULL, IDC_HAND);
  static HCURSOR kCursorAppStarting = LoadCursor(NULL, IDC_APPSTARTING);

  if (cursor == kCursorArrow)
    return JNI_STATIC(DEFAULT_CURSOR);
  if (cursor == kCursorIBeam)
    return JNI_STATIC(TEXT_CURSOR);
  if (cursor == kCursorWait)
    return JNI_STATIC(WAIT_CURSOR);
  if (cursor == kCursorCross)
    return JNI_STATIC(CROSSHAIR_CURSOR);
  if (cursor == kCursorHand)
    return JNI_STATIC(HAND_CURSOR);

  return JNI_STATIC(DEFAULT_CURSOR);
}
