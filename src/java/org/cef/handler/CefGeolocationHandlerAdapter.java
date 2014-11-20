// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.handler;

import org.cef.browser.CefBrowser;
import org.cef.callback.CefGeolocationCallback;

/**
 * An abstract adapter class for receiving geolocation requests.
 * The methods in this class are empty.
 * This class exists as convenience for creating handler objects.
 */
public abstract class CefGeolocationHandlerAdapter
    implements CefGeolocationHandler {
  @Override
  public boolean onRequestGeolocationPermission(
      CefBrowser browser,
      String requesting_url,
      int request_id,
      CefGeolocationCallback callback) {
    return false;
  }

  @Override
  public void onCancelGeolocationPermission(CefBrowser browser,
                                            String requesting_url,
                                            int request_id) {
  }
}
