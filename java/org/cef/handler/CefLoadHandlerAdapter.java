// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.handler;

import org.cef.browser.CefBrowser;

/**
 * An abstract adapter class for receiving load events.
 * The methods in this class are empty.
 * This class exists as convenience for creating handler objects.
 */
public abstract class CefLoadHandlerAdapter implements CefLoadHandler {
  @Override
  public void onLoadingStateChange(CefBrowser browser,
                                   boolean isLoading,
                                   boolean canGoBack,
                                   boolean canGoForward) {
  }

  @Override
  public void onLoadStart(CefBrowser browser, int frameIdentifer) {
  }

  @Override
  public void onLoadEnd(CefBrowser browser,
                        int frameIdentifier,
                        int httpStatusCode) {
  }

  @Override
  public void onLoadError(CefBrowser browser,
                          int frameIdentifer,
                          ErrorCode errorCode,
                          String errorText,
                          String failedUrl) {
  }
}
