// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef;

import java.awt.Cursor;

/**
 * Implement this interface to handle events when window rendering is disabled.
 * The methods of this class will be called on the UI thread.
 */
public interface CefRenderHandler {

  /**
   * Handle cursor changes.
   * @param client The client generating the event.
   * @param cursor The new cursor.
   */
  public void onCursorChange(CefClient client,
                             Cursor cursor);
}
