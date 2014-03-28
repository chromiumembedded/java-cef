// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "render_handler.h"
#include <gdk/gdk.h>
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

  const GdkCursorType cursor_type = gdk_cursor_get_cursor_type(cursor);
  switch (cursor_type) {
    case GDK_CROSSHAIR:
      return JNI_STATIC(CROSSHAIR_CURSOR);
    case GDK_RIGHT_SIDE:
      return JNI_STATIC(E_RESIZE_CURSOR);
    case GDK_HAND1:
    case GDK_HAND2:
      return JNI_STATIC(HAND_CURSOR);
    case GDK_FLEUR:
      return JNI_STATIC(MOVE_CURSOR);
    case GDK_TOP_SIDE:
      return JNI_STATIC(N_RESIZE_CURSOR);
    case GDK_TOP_RIGHT_CORNER:
      return JNI_STATIC(NE_RESIZE_CURSOR);
    case GDK_TOP_LEFT_CORNER:
      return JNI_STATIC(NW_RESIZE_CURSOR);
    case GDK_BOTTOM_SIDE:
      return JNI_STATIC(S_RESIZE_CURSOR);
    case GDK_BOTTOM_RIGHT_CORNER:
      return JNI_STATIC(SE_RESIZE_CURSOR);
    case GDK_BOTTOM_LEFT_CORNER:
      return JNI_STATIC(SW_RESIZE_CURSOR);
    case GDK_XTERM:
      return JNI_STATIC(TEXT_CURSOR);
    case GDK_LEFT_SIDE:
      return JNI_STATIC(W_RESIZE_CURSOR);
    case GDK_WATCH:
      return JNI_STATIC(WAIT_CURSOR);
    default:
      break;
  }

  return JNI_STATIC(DEFAULT_CURSOR);
}

