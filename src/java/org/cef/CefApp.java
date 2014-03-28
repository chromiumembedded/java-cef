// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef;

import java.awt.Canvas;
import java.io.File;
import java.io.FilenameFilter;

/**
 * Exposes static methods for managing the global CEF context.
 */
public class CefApp extends CefAppHandlerAdapter {
  /**
   * According the singleton pattern, this attribute keeps
   * one single object of this class.
   */
  private static CefApp self = null;
  private static CefAppHandler appHandler_ = null;
  private boolean isInitialized_ = false;

  /**
   * To get an instance of this class, use the method
   * getInstance() instead of this CTOR.
   * 
   * The CTOR is called by getInstance() as needed and
   * loads all required JCEF libraries.
   * 
   * @throws UnsatisfiedLinkError
   */
  private CefApp(String [] args) throws UnsatisfiedLinkError {
    super(args);
    if (OS.isWindows()) {
      System.loadLibrary("icudt");
      System.loadLibrary("libcef");
    } else if (OS.isLinux()) {
      System.loadLibrary("cef");
    }
    System.loadLibrary("jcef");
    if (appHandler_ == null) {
      appHandler_ = this;
    }
  }

  @Override
  protected void finalize() throws Throwable {
    self = null;
  }

  public static void addAppHandler(CefAppHandler appHandler) {
    appHandler_ = appHandler;
  }

  /**
   * Get an instance of this class.
   * @return an instance of this class
   * @throws UnsatisfiedLinkError
   */
  public static synchronized CefApp getInstance() throws UnsatisfiedLinkError {
     return getInstance(null);
  }

  public static synchronized CefApp getInstance(String [] args) throws UnsatisfiedLinkError {
    if (self == null) {
      self = new CefApp(args);
    }
    return self;
  }

  /**
   * Initialize the context.
   * @return true on success
   */
  public final boolean initialize(String cachePath, boolean osr) {
    if (isInitialized_)
      return true;
    String library_path = getJcefLibPath();
    System.out.println("initialize on " + Thread.currentThread() +
                       " with library path " + library_path);
    isInitialized_ = N_Initialize(library_path, appHandler_, cachePath, osr);
    return isInitialized_;
  }

  /**
   * Shut down the context.
   */
  public final void shutdown() {
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
  public final void doMessageLoopWork() {
    try {
      N_DoMessageLoopWork();
    } catch (UnsatisfiedLinkError err) {
      err.printStackTrace();
    }
  }

  /**
   * Create a new browser.
   */
  public final CefBrowser createBrowser(CefClientHandler handler, long windowHandle, String url, boolean transparent, Canvas canvas) {
    try {
      return N_CreateBrowser(handler, windowHandle, url, transparent, canvas);
    } catch (UnsatisfiedLinkError err) {
      err.printStackTrace();
    }
    return null;
  }
  
  /**
   * Returns the native window handle for the specified native surface handle.
   */
  public final long getWindowHandle(long surfaceHandle) {
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
  private final String getJcefLibPath() {
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
      if (found != null && found.length != 0)
        return path;
    }
    return library_path;
  }

  private final native boolean N_Initialize(String pathToJavaDLL, CefAppHandler appHandler, String cachePath, boolean osr);
  private final native void N_Shutdown();
  private final native void N_DoMessageLoopWork();
  private final native CefBrowser N_CreateBrowser(CefClientHandler handler, long windowHandle, String url, boolean transparent, Canvas canvas);
  private final native long N_GetWindowHandle(long surfaceHandle);
}
