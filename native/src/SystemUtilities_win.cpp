// Copyright (c) 2010 Lime Wire, LLC. All rights reserved. Use of this source
// code is governed by a BSD-style license that can be found in the LICENSE file.

#include "SystemUtilities_win.h"
#include <jawt_md.h>
#include <string>

// A Windows DLL has a DllMain method that Windows calls when it loads the DLL
BOOL APIENTRY DllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
  
	// Indicate success
	return TRUE;
}

typedef jboolean(JNICALL *JawtGetAwtSignature)(JNIEnv*, JAWT*);
typedef jint (JNICALL GetCreatedJavaVMs)(JavaVM**, jsize, jsize*);

JAWT* GetJAWT(JNIEnv *e, jclass c, jobject frame)
{
	extern wchar_t g_JavaPath[];
  
  // Make a JAWT structure that will tell Java we're using Java 1.4
	JAWT* awt = new JAWT();
	awt->version = JAWT_VERSION_1_4;
  
	// Load jawt.dll into our process space
	wchar_t path[MAX_PATH];
	wsprintf(path, L"%s\\jawt.dll", g_JavaPath);
  
	// If the DLL is already in our process space, LoadLibrary() will just get
	// its handle
	HMODULE module = LoadLibrary(path);
	if (module) {
		// Get a function pointer to JAWT_GetAWT() in the DLL
		JawtGetAwtSignature JawtGetAwt =
    (JawtGetAwtSignature)GetProcAddress(module, "_JAWT_GetAWT@8");
		if (JawtGetAwt) {
			// Access Java's Active Widget Toolkit
			jboolean result = JawtGetAwt(e, awt);
			if (result != JNI_FALSE)
        return awt;
    }
  }
  
  return NULL;
}

HWND GetJavaWindowHandle(JNIEnv *e, jclass c, jobject frame)
{
  // Make a variable for the window handle we'll get
  HWND window = NULL;
  
  JAWT* awt = GetJAWT(e, c, frame);
  if (!awt)
		return NULL;
  
  // Get the drawing surface
  JAWT_DrawingSurface *surface = awt->GetDrawingSurface(e, frame);
  if (surface) {
    // Lock the drawing surface
    jint lock = surface->Lock(surface);
    if ((lock & JAWT_LOCK_ERROR) == 0) {
			// Get the drawing surface information
      JAWT_DrawingSurfaceInfo *info =
      surface->GetDrawingSurfaceInfo(surface);
      if (info) {
        // Get the Windows-specific drawing surface information
        JAWT_Win32DrawingSurfaceInfo *win =
        (JAWT_Win32DrawingSurfaceInfo*)info->platformInfo;
        if (win) {
          // Get the window handle
          window = win->hwnd;
        }
      }
      
      // Unlock the drawing surface
      surface->Unlock(surface);
    }
    
    // Free the drawing surface
    awt->FreeDrawingSurface(surface);
  }
  
	delete awt;
  
	// Return the window handle Java told us
	return window;
}

JavaVM* GetJavaVM()
{
	extern wchar_t g_JavaPath[];
  
  // Load jvm.dll into our process space
	wchar_t path[MAX_PATH];
	wsprintf(path, L"%s\\jvm.dll", g_JavaPath);
  
	// If the DLL is already in our process space, LoadLibrary() will just get
	// its handle
	HMODULE module = LoadLibrary(path);
	if(!module)
	{
		// Try a different path for Java 6 and newer.
		wsprintf(path, L"%s\\client\\jvm.dll", g_JavaPath);
		module = LoadLibrary(path);
	}
  
	if (module) {
		GetCreatedJavaVMs *pFunc =
    (GetCreatedJavaVMs*)GetProcAddress(module, "JNI_GetCreatedJavaVMs");
		if(pFunc) {
			JavaVM* jvm;
			jint nSize = 1, nVms;
			if(pFunc(&jvm, nSize, &nVms) == 0)
				return jvm;
		}
	}
  
	return NULL;
}
