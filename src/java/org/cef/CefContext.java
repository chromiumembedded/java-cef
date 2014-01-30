// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef;

import java.io.File;
import java.io.FilenameFilter;

/**
 * Exposes static methods for managing the global CEF context.
 */
public class CefContext {
  /**
   * Initialize the context.
   * @return true on success
   */
  public static final boolean initialize(String cachePath) {
    String library_path = getJcefLibPath();
    System.out.println("initialize on " + Thread.currentThread() +
                       " with library path " + library_path);
    try {
      String os = System.getProperty("os.name");
      if (os.startsWith("Windows")) {
        System.loadLibrary("icudt");
        System.loadLibrary("libcef");
        System.loadLibrary("jcef");
      } else if (os.startsWith("Linux")) {
        System.loadLibrary("cef");
        System.loadLibrary("jcef");
      } else if (os.startsWith("Mac OS X")) {
        System.loadLibrary("jcef");
      }
      return N_Initialize(library_path, cachePath);
    } catch (UnsatisfiedLinkError err) {
      err.printStackTrace();
    }
    System.exit(0);
    return false;
  }

  /**
   * Shut down the context.
   */
  public static final void shutdown() {
    System.out.println("shutdown on " + Thread.currentThread());
    System.runFinalization();
    try {
      N_Shutdown();
    } catch (UnsatisfiedLinkError err) {
      err.printStackTrace();
    }
  }

  /**
   * Perform a single message loop iteration.
   */
  public static final void doMessageLoopWork() {
    try {
      N_DoMessageLoopWork();
    } catch (UnsatisfiedLinkError err) {
      err.printStackTrace();
    }
  }

  /**
   * Create a new browser.
   */
  public static final CefBrowser createBrowser(CefHandler handler, long windowHandle, String url, boolean transparent) {
    try {
      return N_CreateBrowser(handler, windowHandle, url, transparent);
    } catch (UnsatisfiedLinkError err) {
      err.printStackTrace();
    }
    return null;
  }
  
  /**
   * Returns the native window handle for the specified native surface handle.
   */
  public static final long getWindowHandle(long surfaceHandle) {
    try {
      return N_GetWindowHandle(surfaceHandle);
    } catch (UnsatisfiedLinkError err) {
      err.printStackTrace();
    }
    return 0;
  }

  /**
   * Get the path which contains the jcef library
   * @return The path to the jcef library
   */
  private static final String getJcefLibPath() {
    String library_path = System.getProperty("java.library.path");
    String[] paths = library_path.split(System.getProperty("path.separator"));
    for (String path : paths) {
      File dir = new File(path);
      String[] found = dir.list(new FilenameFilter() {
        @Override
        public boolean accept(File dir, String name) {
          return (name.equalsIgnoreCase("libjcef.dylib") ||
                  name.equalsIgnoreCase("libjcef.so") ||
                  name.equalsIgnoreCase("jcef.dll"));
        }
      });
      if (found.length != 0)
        return path;
    }
    return library_path;
  }

  private static final native boolean N_Initialize(String pathToJavaDLL, String cachePath);
  private static final native void N_Shutdown();
  private static final native void N_DoMessageLoopWork();
  private static final native CefBrowser N_CreateBrowser(CefHandler handler, long windowHandle, String url, boolean transparent);
  private static final native long N_GetWindowHandle(long surfaceHandle);
}
