// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef;

import java.awt.Canvas;

/**
 * Interface representing system dependent methods for the browser.
 */
public interface CefBrowserWindow {
  /**
   * Get the window handle for the given Canvas object.
   * 
   * @param canvas an canvas object
   * @return a window pointer if any
   */
  public long getWindowHandleOfCanvas(Canvas canvas);
}
