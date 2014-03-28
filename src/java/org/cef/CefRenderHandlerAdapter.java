// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef;

import java.awt.Cursor;

/**
 * An abstract adapter class for receiving render events. 
 * The methods in this class are empty.
 * This class exists as convenience for creating handler objects.
 */
public abstract class CefRenderHandlerAdapter implements CefRenderHandler {

  @Override
  public void onCursorChange(CefClient client, 
                             Cursor cursor) {
    return;
  }
}
