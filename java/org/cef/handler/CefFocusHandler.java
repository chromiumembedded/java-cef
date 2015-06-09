// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.handler;

import org.cef.browser.CefBrowser;

/**
 * Implement this interface to handle events related to focus. The methods of
 * this class will be called on the UI thread.
 */
public interface CefFocusHandler {

  /**
   * Focus sources.
   */
  enum FocusSource {
    FOCUS_SOURCE_NAVIGATION, //!< The source is explicit navigation via the API (LoadURL(), etc).
    FOCUS_SOURCE_SYSTEM      //!< The source is a system-generated focus event.
  }

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
   * @param source indicates/ where the focus request is originating from. 
   *
   * @return false to allow the focus to be set or true to cancel setting the focus.
   */
  public boolean onSetFocus(CefBrowser browser, FocusSource source);

  /**
   * Called when the browser component has received focus.
   * @param browser The browser generating the event.
   */
  public void onGotFocus(CefBrowser browser);
}
