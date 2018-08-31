// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "jni_util.h"

#include <X11/Xlib.h>
#include <assert.h>
#include <jawt.h>
#include <jawt_md.h>

static Display* x11_display = NULL;

unsigned long GetDrawableOfCanvas(jobject canvas, JNIEnv* env) {
  JAWT awt;
  JAWT_DrawingSurface* ds;
  JAWT_DrawingSurfaceInfo* dsi;
  JAWT_X11DrawingSurfaceInfo* dsi_x11;
  jint lock;

  // Get the AWT.
  awt.version = JAWT_VERSION_1_4;
  JAWT_GetAWT(env, &awt);

  // Get the drawing surface.
  ds = awt.GetDrawingSurface(env, canvas);
  assert(ds != NULL);

  // Lock the drawing surface.
  // May fail during shutdown.
  lock = ds->Lock(ds);
  if (lock & JAWT_LOCK_ERROR) {
    return 0;
  }

  // Get the drawing surface info.
  dsi = ds->GetDrawingSurfaceInfo(ds);
  if (dsi == NULL) {
    // Unlock the drawing surface
    ds->Unlock(ds);
    return 0;
  }

  // Get the platform-specific drawing info.
  dsi_x11 = (JAWT_X11DrawingSurfaceInfo*)dsi->platformInfo;
  Drawable result = dsi_x11->drawable;

  // Also store the X11 connection so we can use it later (see below)
  x11_display = dsi_x11->display;

  // Free the drawing surface info
  ds->FreeDrawingSurfaceInfo(dsi);
  // Unlock the drawing surface
  ds->Unlock(ds);
  // Free the drawing surface
  awt.FreeDrawingSurface(ds);

  return result;
}

void X_XMoveResizeWindow(unsigned long browserHandle,
                         int x,
                         int y,
                         unsigned int width,
                         unsigned int height) {
  XMoveResizeWindow(x11_display, browserHandle, 0, 0, width, height);
}

void X_XReparentWindow(unsigned long browserHandle,
                       unsigned long parentDrawable) {
  XReparentWindow(x11_display, browserHandle, parentDrawable, 0, 0);
}

void X_XSetInputFocus(unsigned long browserHandle) {
  XSetInputFocus(x11_display, browserHandle, RevertToParent, CurrentTime);
}

void X_XSetInputFocusParent(unsigned long browserHandle) {
  Window root_win;
  Window parent_win;
  Window* child_windows;
  unsigned int num_child_windows;
  XQueryTree(x11_display, browserHandle, &root_win, &parent_win, &child_windows,
             &num_child_windows);
  XFree(child_windows);

  XSetInputFocus(x11_display, parent_win, RevertToParent, CurrentTime);
}
