// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef;

/**
 * Implement this interface to handle events related to browser life span. The
 * methods of this class will be called on the UI thread unless otherwise
 * indicated.
 */
public interface CefLifeSpanHandler {

  /**
   * Handle creation of a new browser window.
   * @param browser The browser generating the event.
   */
  public void onAfterCreated(CefBrowser browser);
}
