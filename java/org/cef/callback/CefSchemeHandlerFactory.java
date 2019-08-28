// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.callback;

import org.cef.browser.CefBrowser;
import org.cef.browser.CefFrame;
import org.cef.handler.CefResourceHandler;
import org.cef.network.CefRequest;

/**
 * Class that creates CefResourceHandler instances for handling scheme requests.
 * The methods of this class will always be called on the IO thread.
 */
public interface CefSchemeHandlerFactory {
    /**
     * Return a new resource handler instance to handle the request or NULL to allow default
     * handling of the request.
     *
     * @param browser The corresponding browser, or NULL if the request did not originate from a
     *         browser window (for example, if the request came from CefURLRequest).
     * @param frame The frame generating the event, or NULL if the request did not originate from a
     *         browser window (for example, if the request came from CefURLRequest). Instance only
     *         valid within the scope of this  method.
     * @param schemeName Name of the scheme being created.
     * @param request The request itself. Cannot be modified in this callback. Instance only valid
     *         within the scope of this method.
     */
    public CefResourceHandler create(
            CefBrowser browser, CefFrame frame, String schemeName, CefRequest request);
}
