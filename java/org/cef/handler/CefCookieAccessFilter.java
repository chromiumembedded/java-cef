// Copyright (c) 2019 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.handler;

import org.cef.browser.CefBrowser;
import org.cef.browser.CefFrame;
import org.cef.network.CefCookie;
import org.cef.network.CefRequest;
import org.cef.network.CefResponse;

/**
 * Implement this interface to filter cookies that may be sent or received from resource requests.
 * The methods of this class will be called on the IO thread.
 */
public interface CefCookieAccessFilter {
    /**
     * Called on the IO thread before a resource request is sent. The |browser| and |frame| values
     * represent the source of the request, and may be null for requests originating from service
     * workers or CefURLRequest.
     *
     * @param browser The corresponding browser.
     * @param frame The frame generating the event. Instance only valid within the scope of this
     *         method.
     * @param request The request itself. Cannot be modified in this callback. Instance only valid
     *         within the scope of this method.
     * @param cookie The cookie that will be sent with the request. Cannot be modified in this
     *         callback. Instance only valid within the scope of this method.
     * @return True if the cookie can be sent or false otherwise.
     */
    boolean canSendCookie(CefBrowser browser, CefFrame frame, CefRequest request, CefCookie cookie);

    /**
     * Called on the IO thread after a resource response is received. The |browser| and |frame|
     * values represent the source of the request, and may be null for requests originating from
     * service workers or CefURLRequest.
     *
     * @param browser The corresponding browser.
     * @param frame The frame generating the event. Instance only valid within the scope of this
     *         method.
     * @param request The request itself. Cannot be modified in this callback. Instance only valid
     *         within the scope of this method.
     * @param response The request response. Cannot be modified in this callback. Instance only
     *         valid within the scope of this method.
     * @param cookie The cookie that will be sent with the request. Cannot be modified in this
     *         callback. Instance only valid within the scope of this method.
     * @return True if the cookie can be saved or false otherwise.
     */
    boolean canSaveCookie(CefBrowser browser, CefFrame frame, CefRequest request,
            CefResponse response, CefCookie cookie);
}
