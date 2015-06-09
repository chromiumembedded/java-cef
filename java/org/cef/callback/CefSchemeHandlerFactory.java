// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.callback;

import org.cef.browser.CefBrowser;
import org.cef.handler.CefResourceHandler;
import org.cef.network.CefRequest;

/**
 * Class that creates CefResourceHandler instances for handling scheme requests.
 * The methods of this class will always be called on the IO thread.
 */
public interface CefSchemeHandlerFactory {
  /**
   * Return a new resource handler instance to handle the request or an empty
   * reference to allow default handling of the request. |browser| and |frame|
   * will be the browser window and frame respectively that originated the
   * request or NULL if the request did not originate from a browser window
   * (for example, if the request came from CefURLRequest). The |request| object
   * passed to this method will not contain cookie data.
   */
  public CefResourceHandler create(CefBrowser browser,
                                   String schemeName,
                                   CefRequest request);
}
