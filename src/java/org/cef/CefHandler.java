// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef;

import java.awt.Point;
import java.awt.Rectangle;
import java.nio.ByteBuffer;

/**
 * Callbacks from a browser.
 */

public interface CefHandler {
  /**
   * Handle creation of a new browser window.
   * @param browser The browser generating the event.
   */
  public void onAfterCreated(CefBrowser browser);
  
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
   * Retrieve the view rectangle.
   * @param browser The browser generating the event.
   * @return The view rectangle.
   */
  public Rectangle getViewRect(CefBrowser browser);
  
  /**
   * Retrieve the screen point for the specified view point.
   * @param browser The browser generating the event.
   * @param viewPoint The point in the view.
   * @return The screen point.
   */
  public Point getScreenPoint(CefBrowser browser, Point viewPoint);
  
  /**
   * Show or hide the popup window.
   * @param browser The browser generating the event.
   * @param show True if the popup window is being shown.
   */
  public void onPopupShow(CefBrowser browser,
                          boolean show);

  /**
   * Size the popup window.
   * @param browser The browser generating the event.
   * @param size Size of the popup window.
   */
  public void onPopupSize(CefBrowser browser,
                          Rectangle size);

  /**
   * Handle painting.
   * @param browser The browser generating the event.
   * @param popup True if painting a popup window.
   * @param dirtyRects Array of dirty regions.
   * @param buffer Pixel buffer for the whole window.
   * @param width Width of the buffer.
   * @param height Height of the buffer.
   */
  public void onPaint(CefBrowser browser,
                      boolean popup,
                      Rectangle[] dirtyRects,
                      ByteBuffer buffer,
                      int width,
                      int height);

  /**
   * Handle cursor changes.
   * @param browser The browser generating the event.
   * @param cursorType Cursor type.
   */
  public void onCursorChange(CefBrowser browser,
                             int cursorType);

  /**
   * Called when the browser component is about to loose focus.
   * For instance, if focus was on the last HTML element and the
   * user pressed the TAB key.
   * @param browser The browser generating the event.
   * @param next will be true if the browser is giving focus to the
   *        next component and false if the browser is giving focus
   *        to the previous component.
   */
  public void onTakeFocus(CefBrowser browser, boolean next);

  /**
   * Called when the browser component is requesting focus.
   * @param browser The browser generating the event.
   * @param source indicates where the focus request is originating from.
   *
   * @return false to allow the focus to be set or true to cancel setting the focus.
   */
  public boolean onSetFocus(CefBrowser browser, long source);

  /**
   * Called when the browser component has received focus.
   * @param browser The browser generating the event.
   */
  public void onGotFocus(CefBrowser browser);
  
  /**
   * Called when the browser receives a JavaScript query.
   * @param browser The browser generating the event.
   * @param query_id The unique ID for the query.
   * @param persistent True if the query is persistent.
   * @param callback Object used to continue or cancel the query asynchronously.
   */
  public void onQuery(CefBrowser browser,
                      long query_id,
                      String request,
                      boolean persistent,
                      CefQueryCallback callback);

  /**
   * Called when a pending JavaScript query is canceled.
   * @param browser The browser generating the event.
   * @param query_id The unique ID for the query.
   */
  public void onQueryCanceled(CefBrowser browser,
                              long query_id);
}
