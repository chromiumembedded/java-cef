// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.browser;

import java.awt.Component;

/**
 * Interface representing system dependent methods for the browser.
 */
public interface CefBrowserWindow {
  /**
   * Get the window handle for the given UI object.
   * 
   * @param comp a UI component
   * @return a window pointer if any
   */
  public long getWindowHandle(Component comp);
}
