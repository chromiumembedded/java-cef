// Copyright (c) 2010 Lime Wire, LLC. All rights reserved. Use of this source
// code is governed by a BSD-style license that can be found in the LICENSE file.

#include "SystemUtilities_mac.h"
#include <jawt.h>
#include <jawt_md.h>

JavaVM* GetJavaVM()
{
	JavaVM* jvm;
  jint nSize = 1, nVms;
  if(JNI_GetCreatedJavaVMs(&jvm, nSize, &nVms) == 0)
    return jvm;
  
	return NULL;
}

NSView* GetNSView(JNIEnv *env, jobject javaWindow)
{
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    
    NSView* view = NULL;
    JAWT awt;
    JAWT_DrawingSurface* ds;
    JAWT_DrawingSurfaceInfo* dsi;
    JAWT_MacOSXDrawingSurfaceInfo* dsi_osx;
    jboolean result;
    jint lock;
    
    // Get the AWT
    awt.version = JAWT_VERSION_1_3;
    result = JAWT_GetAWT(env, &awt);
    if (result != JNI_FALSE)
    {
        // Get the drawing surface
        ds = awt.GetDrawingSurface(env, javaWindow);
        if (ds != NULL)
        {
            // Lock the drawing surface
            lock = ds->Lock(ds);
            if ((lock & JAWT_LOCK_ERROR) == 0)
            {
                // Get the drawing surface info
                dsi = ds->GetDrawingSurfaceInfo(ds);
                if (dsi != NULL)
                {
                    // Get the platform-specific drawing info
                    dsi_osx = (JAWT_MacOSXDrawingSurfaceInfo*)dsi->platformInfo;
                    if (dsi_osx != NULL)
                    {
                        view = dsi_osx->cocoaViewRef;
                    }
                    
                    // Free the drawing surface info
                    ds->FreeDrawingSurfaceInfo(dsi);
                }
            }              
            
            // Unlock the drawing surface
            ds->Unlock(ds);

            // Free the drawing surface
            awt.FreeDrawingSurface(ds);
        }
    }

    [pool release];	

    return view;
}

