// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "jni_util.h"
#include <jawt.h>
#include <jawt_md.h>

void AddLayerToComponent(jobject parent, JNIEnv *env, CefWindowHandle windowHandle){
#ifdef __MAC_10_7
  JAWT awt;
  JAWT_DrawingSurface* ds;
  JAWT_DrawingSurfaceInfo* dsi;
  id<JAWT_SurfaceLayers> dsi_mac;
  jboolean result;
  jint lock;

  // Get the AWT
  awt.version = JAWT_VERSION_1_4 | JAWT_MACOSX_USE_CALAYER;
  result = JAWT_GetAWT(env, &awt);
  assert(result != JNI_FALSE);
  assert(parent != NULL);

  // Get the drawing surface
  ds = awt.GetDrawingSurface(env, parent);
  assert(ds != NULL);

  // Lock the drawing surface
  lock = ds->Lock(ds);
  assert((lock & JAWT_LOCK_ERROR) == 0);

  // Get the drawing surface info
  dsi = ds->GetDrawingSurfaceInfo(ds);
  if (dsi == NULL) {
    NSLog(@"Could not get DrawingSurfaceInfo");
    return;
  }

  // Get the platform-specific drawing info
  dsi_mac = (id<JAWT_SurfaceLayers>)dsi->platformInfo;

  // Set the CALayer corresponding to the component that was passed
  [dsi_mac setLayer:[windowHandle layer]];

  // Free the drawing surface info
  ds->FreeDrawingSurfaceInfo(dsi);

  // Unlock the drawing surface
  ds->Unlock(ds);

  // Free the drawing surface
  awt.FreeDrawingSurface(ds);
#endif
  return;
}
