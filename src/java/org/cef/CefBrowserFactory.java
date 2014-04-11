// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef;

/**
 * Creates a new instance of CefBrowser according the passed values
 */
public class CefBrowserFactory {
  public static CefBrowser create(CefClientHandler clientHandler,
                                  String url,
                                  boolean isOffscreenRendered,
                                  boolean isTransparent) {
    if (isOffscreenRendered)
      return new CefBrowserOsr(clientHandler, url, isTransparent);
    return new CefBrowserWr(clientHandler, url);
  }
}
