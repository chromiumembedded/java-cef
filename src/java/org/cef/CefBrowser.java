// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef;

import java.awt.event.KeyEvent;
import java.awt.event.MouseEvent;
import java.awt.event.MouseWheelEvent;
import java.awt.Rectangle;

/**
 * Interface representing a browser.
 */
public interface CefBrowser {
  /**
   * Close the browser.
   */
  public void close();
  
  /**
   * Go back.
   */
  public void goBack();
  
  /**
   * Go forward.
   */
  public void goForward();
  
  /**
   * Returns the unique browser identifier.
   * @return The browser identifier
	 */
  public int getIdentifier();
  
  /**
   * Load the specified URL in the main frame.
   * @param url The URL to load.
	 */
  public void loadURL(String url);

  /**
   * Notify that the browser was resized.
   */
  public void wasResized();

  /**
   * Invalidate the specified rectangle.
   * @param rect The rectangle to invalidate.
   */
  public void invalidate(Rectangle rect);

  /**
   * Send a key event.
   * @param e The event to send.
   */
  public void sendKeyEvent(KeyEvent e);

  /**
   * Send a mouse event.
   * @param e The event to send.
   */
  public void sendMouseEvent(MouseEvent e);

  /**
   * Send a mouse wheel event.
   * @param e The event to send.
   */
  public void sendMouseWheelEvent(MouseWheelEvent e);
}
