// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.callback;

/**
 * The methods of this interface are triggered by the native code
 * to store and get the JNI counterparts of the JCEF implementation.
 */
public interface CefNative {
  /**
   * Method is called by the native code to store a reference
   * to an implemented native JNI counterpart.
   * 
   * @param identifer The name of the interface class (e.g. CefFocusHandler).
   * @param nativeRef The reference to the native code.
   */
  public void setNativeRef(String identifer, long nativeRef);

  /**
   * Method is called by the native code to get the reference
   * to an previous stored identifier.
   * 
   * @param identifer The name of the interface class (e.g. CefFocusHandler).
   * @return The stored reference value of the native code.
   */
  public long getNativeRef(String identifer);
}
