// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef;

/**
 * Implement this interface to handle events related to browser display state.
 * The methods of this class will be called on the UI thread.
 */
public interface CefDisplayHandler {

  /**
   * Handle address changes.
   * @param browser The browser generating the event.
   * @param url The new address.
   */
  public void onAddressChange(CefBrowser browser, String url);

  /**
   * Handle title changes.
   * @param browser The browser generating the event.
   * @param title The new title.
   */
  public void onTitleChange(CefBrowser browser, String title);
}
