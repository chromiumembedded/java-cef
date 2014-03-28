// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef;

/**
 * An abstract adapter class for receiving display events. 
 * The methods in this class are empty.
 * This class exists as convenience for creating handler objects.
 */
public abstract class CefDisplayHandlerAdapter implements CefDisplayHandler {

  @Override
  public void onAddressChange(CefClient client, String url) {
    return;
  }

  @Override
  public void onTitleChange(CefClient client, String title) {
    return;
  }
}
