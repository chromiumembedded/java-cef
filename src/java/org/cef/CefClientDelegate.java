// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef;

import java.awt.Cursor;

/**
 * Delegate interface implemented by the client owner.
 */

public interface CefClientDelegate {
  /**
   * Focus sources.
   */
  enum FocusSource {
    FOCUS_SOURCE_NAVIGATION, //!< The source is explicit navigation via the API (LoadURL(), etc).
    FOCUS_SOURCE_SYSTEM      //!< The source is a system-generated focus event.
  }

  /**
   * Handle address changes.
   * @param client The client generating the event.
   * @param url The new address.
   */
  public void onAddressChange(CefClient client, String url);

  /**
   * Handle title changes.
   * @param client The client generating the event.
   * @param title The new title.
   */
  public void onTitleChange(CefClient client, String title);

  /**
   * Handle cursor changes.
   * @param client The client generating the event.
   * @param cursor The new cursor.
   */
  public void onCursorChange(CefClient client,
                             Cursor cursor);

  /**
   * Called when the browser component is about to loose focus. 
   * For instance, if focus was on the last HTML element and the 
   * user pressed the TAB key.
   * @param client The client generating the event.
   * @param next will be true if the browser is giving focus to the 
   *        next component and false if the browser is giving focus 
   *        to the previous component.
   */
  public void onTakeFocus(CefClient client, boolean next);

  /**
   * Called when the browser component is requesting focus.
   * @param client The client generating the event.
   * @param source indicates/ where the focus request is originating from. 
   *
   * @return false to allow the focus to be set or true to cancel setting the focus.
   */
  public boolean onSetFocus(CefClient client, FocusSource source);

  /**
   * Called when the browser component has received focus.
   * @param client The client generating the event.
   */
  public void onGotFocus(CefClient client);
}
