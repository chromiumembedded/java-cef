// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.handler;

import org.cef.browser.CefBrowser;
import org.cef.callback.CefDragData;

/**
 * Implement this interface to handle events related to dragging. The methods of
 * this class will be called on the UI thread.
 */
public interface CefDragHandler {
  /**
   * 
   */
  public static final class DragOperationMask {
    public static final int DRAG_OPERATION_NONE    = 0;
    public static final int DRAG_OPERATION_COPY    = 1;
    public static final int DRAG_OPERATION_LINK    = 2;
    public static final int DRAG_OPERATION_GENERIC = 4;
    public static final int DRAG_OPERATION_PRIVATE = 8;
    public static final int DRAG_OPERATION_MOVE    = 16;
    public static final int DRAG_OPERATION_DELETE  = 32;
    public static final int DRAG_OPERATION_EVERY   = Integer.MAX_VALUE;
  }

  /**
   * Called when an external drag event enters the browser window.
   * 
   * @param browser The corresponding browser.
   * @param dragData contains the drag event data
   * @param mask represents the type of drag operation. See DragOperationMask for possible values.
   * @return false for default drag handling behavior or true to
   * cancel the drag event.
   */
  public boolean onDragEnter(CefBrowser browser, 
                             CefDragData dragData,
                             int mask);
}
