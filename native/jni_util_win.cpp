// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "jni_util.h"

#include <assert.h>
#include <jawt.h>
#include <jawt_md.h>

HWND GetHwndOfCanvas(jobject canvas, JNIEnv* env) {
  JAWT awt;
  JAWT_DrawingSurface* ds;
  JAWT_DrawingSurfaceInfo* dsi;
  JAWT_Win32DrawingSurfaceInfo* dsi_win;
  jboolean bGetAwt;
  jint lock;

  // Get the AWT.
  awt.version = JAWT_VERSION_1_4;
  bGetAwt = JAWT_GetAWT(env, &awt);
  assert(bGetAwt != JNI_FALSE);

  // Get the drawing surface.
  ds = awt.GetDrawingSurface(env, canvas);
  assert(ds != nullptr);

  // Lock the drawing surface.
  // May fail during shutdown.
  lock = ds->Lock(ds);
  if (lock & JAWT_LOCK_ERROR) {
    return 0;
  }

  // Get the drawing surface info.
  dsi = ds->GetDrawingSurfaceInfo(ds);
  if (dsi == nullptr) {
    // Unlock the drawing surface
    ds->Unlock(ds);
    return 0;
  }

  // Get the platform-specific drawing info.
  dsi_win = (JAWT_Win32DrawingSurfaceInfo*)dsi->platformInfo;
  HWND result = dsi_win->hwnd;

  // Free the drawing surface info
  ds->FreeDrawingSurfaceInfo(dsi);
  // Unlock the drawing surface
  ds->Unlock(ds);
  // Free the drawing surface
  awt.FreeDrawingSurface(ds);

  return result;
}
