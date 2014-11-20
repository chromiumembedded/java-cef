// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.handler;

import org.cef.network.CefCookieManager;

/**
 * Implement this interface to provide handler implementations.
 */
public interface CefRequestContextHandler {

  /**
   * Called on the IO thread to retrieve the cookie manager. The global cookie
   * manager will be used if this method returns NULL.
   */
  public CefCookieManager getCookieManager();
}
