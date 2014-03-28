// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "render_handler.h"

#import <Cocoa/Cocoa.h>

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

  if ([cursor isEqual: [NSCursor IBeamCursor]])
    return JNI_STATIC(TEXT_CURSOR);
  if ([cursor isEqual: [NSCursor crosshairCursor]])
    return JNI_STATIC(CROSSHAIR_CURSOR);
  if ([cursor isEqual: [NSCursor dragCopyCursor]] ||
      [cursor isEqual: [NSCursor dragLinkCursor]])
    return JNI_STATIC(MOVE_CURSOR);
  if ([cursor isEqual: [NSCursor pointingHandCursor]])
    return JNI_STATIC(HAND_CURSOR);
  if ([cursor isEqual: [NSCursor resizeLeftCursor]])
    return JNI_STATIC(W_RESIZE_CURSOR);
  if ([cursor isEqual: [NSCursor resizeRightCursor]])
    return JNI_STATIC(E_RESIZE_CURSOR);
  if ([cursor isEqual: [NSCursor resizeUpCursor]])
    return JNI_STATIC(N_RESIZE_CURSOR);
  if ([cursor isEqual: [NSCursor resizeDownCursor]])
    return JNI_STATIC(S_RESIZE_CURSOR);

  return JNI_STATIC(DEFAULT_CURSOR);
}
