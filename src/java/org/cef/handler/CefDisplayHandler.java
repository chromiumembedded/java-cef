// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.handler;

import org.cef.browser.CefBrowser;

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

  /**
   * Called when the browser is about to display a tooltip.
   *
   * @param browser The browser generating the event.
   * @param text Contains the text that will be displayed in the tooltip.
   * @return To handle the display of the tooltip yourself return true.
   */
  public boolean onTooltip(CefBrowser browser, String text);

  /**
   * Called when the browser receives a status message. 
   *
   * @param browser The browser generating the event.
   * @param value Contains the text that will be displayed in the status message.
   */
  public void onStatusMessage(CefBrowser browser, String value);

  /**
   * Called to display a console message.
   *
   * @param browser The browser generating the event.
   * @param message 
   * @param source
   * @param line
   * @return true to stop the message from being output to the console.
   */
  public boolean onConsoleMessage(CefBrowser browser,
                                  String message,
                                  String source,
                                  int line);
}
