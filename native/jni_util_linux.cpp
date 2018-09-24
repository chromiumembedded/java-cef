// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "jni_util.h"

#include <assert.h>
#include <jawt.h>
#include <jawt_md.h>
#include <X11/Xlib.h>

#include "temp_window.h"

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
  ::Display* xdisplay = (::Display*)TempWindow::GetDisplay();
  XMoveResizeWindow(xdisplay, browserHandle, 0, 0, width, height);
}

void X_XReparentWindow(unsigned long browserHandle,
                       unsigned long parentDrawable) {
  ::Display* xdisplay = (::Display*)TempWindow::GetDisplay();
  XReparentWindow(xdisplay, browserHandle, parentDrawable, 0, 0);
}

void X_XSetInputFocus(unsigned long browserHandle) {
  ::Display* xdisplay = (::Display*)TempWindow::GetDisplay();
  XSetInputFocus(xdisplay, browserHandle, RevertToParent, CurrentTime);
}

void X_XSetInputFocusParent(unsigned long browserHandle) {
  ::Display* xdisplay = (::Display*)TempWindow::GetDisplay();

  Window root_win;
  Window parent_win;
  Window* child_windows;
  unsigned int num_child_windows;
  XQueryTree(xdisplay, browserHandle, &root_win, &parent_win, &child_windows,
             &num_child_windows);
  XFree(child_windows);

  XSetInputFocus(xdisplay, parent_win, RevertToParent, CurrentTime);
}
