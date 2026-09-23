// Copyright (c) 2026 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include <jni.h>

#include <OpenGL/CGLIOSurface.h>
#include <OpenGL/gl3.h>
#include <OpenGL/OpenGL.h>

extern "C" JNIEXPORT jint JNICALL
Java_org_cef_handler_CefMacOsIOSurface_bindToCurrentTexture(JNIEnv*,
                                                            jclass,
                                                            jlong io_surface,
                                                            jint width,
                                                            jint height) {
  if (io_surface == 0 || width <= 0 || height <= 0) {
    return kCGLBadValue;
  }

  CGLContextObj context = CGLGetCurrentContext();
  if (context == nullptr) {
    return kCGLBadContext;
  }

  return CGLTexImageIOSurface2D(
      context,
      GL_TEXTURE_RECTANGLE,
      GL_RGBA,
      width,
      height,
      GL_BGRA,
      GL_UNSIGNED_INT_8_8_8_8_REV,
      reinterpret_cast<IOSurfaceRef>(io_surface),
      0);
}
