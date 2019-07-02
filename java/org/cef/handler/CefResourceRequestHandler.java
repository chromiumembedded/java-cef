// Copyright (c) 2019 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.handler;

import org.cef.browser.CefBrowser;
import org.cef.browser.CefFrame;
import org.cef.callback.CefRequestCallback;
import org.cef.misc.BoolRef;
import org.cef.misc.StringRef;
import org.cef.network.CefRequest;
import org.cef.network.CefResponse;
import org.cef.network.CefURLRequest;

/**
 * Implement this interface to handle events related to browser requests. The methods of this class
 * will be called on the IO thread unless otherwise indicated.
 */
public interface CefResourceRequestHandler {
    /**
     * Called on the IO thread before a resource is loaded. The |browser| and |frame| values
     * represent the source of the request, and may be null for requests originating from service
     * workers or CefURLRequest.
     *
     * @param browser The corresponding browser.
     * @param frame The frame generating the event. Instance only valid within the scope of this
     *         method.
     * @param request The request itself. Cannot be modified in this callback. Instance only valid
     *         within the scope of this method.
     * @return A CefCookieAccessFilter instance or null.
     */
    CefCookieAccessFilter getCookieAccessFilter(
            CefBrowser browser, CefFrame frame, CefRequest request);

    /**
     * Called on the IO thread before a resource request is loaded. The |browser| and |frame| values
     * represent the source of the request, and may be null for requests originating from service
     * workers or CefURLRequest. To redirect or change the resource load optionally modify
     * |request|. Modification of the request URL will be treated as a redirect.
     *
     * @param browser The corresponding browser.
     * @param frame The frame generating the event. Instance only valid within the scope of this
     *         method.
     * @param request The request itself. May be modified in this callback. Instance only valid
     *      within the scope of this method.
     * @return To cancel the request return true otherwise return false.
     */
    boolean onBeforeResourceLoad(CefBrowser browser, CefFrame frame, CefRequest request);

    /**
     * Called on the IO thread before a resource is loaded. The |browser| and |frame| values
     * represent the source of the request, and may be null for requests originating from service
     * workers or CefURLRequest.
     *
     * @param browser The corresponding browser.
     * @param frame The frame generating the event. Instance only valid within the scope of this
     *         method.
     * @param request The request itself. Cannot be modified in this callback. Instance only valid
     *         within the scope of this method.
     * @return A CefResourceHandler instance or null.
     */
    CefResourceHandler getResourceHandler(CefBrowser browser, CefFrame frame, CefRequest request);

    /**
     * Called on the IO thread when a resource load is redirected. The |browser| and |frame| values
     * represent the source of the request, and may be null for requests originating from service
     * workers or CefURLRequest. The |request| parameter will contain the old URL and other
     * request-related information.
     *
     * @param browser The corresponding browser.
     * @param frame The frame generating the event. Instance only valid within the scope of this
     *         method.
     * @param request The request itself. Cannot be modified in this callback. Instance only valid
     *         within the scope of this method.
     * @param response The response that resulted in the redirect. Cannot be modified in this
     *         callback. Instance only valid within the scope of this method.
     * @param new_url Contains the new URL and can be changed if desired.
     */
    void onResourceRedirect(CefBrowser browser, CefFrame frame, CefRequest request,
            CefResponse response, StringRef new_url);

    /**
     * Called on the IO thread when a resource response is received. The |browser| and |frame|
     * values represent the source of the request, and may be null for requests originating from
     * service workers or CefURLRequest. To allow the resource load to proceed without modification
     * return false. To redirect or retry the resource load optionally modify |request| and return
     * true. Modification of the request URL will be treated as a redirect. Requests handled using
     * the default network loader cannot be redirected in this callback.
     *
     * @param browser The corresponding browser.
     * @param frame The frame generating the event. Instance only valid within the scope of this
     *         method.
     * @param request The request itself. May be modified in this callback. Instance only valid
     *         within the scope of this method.
     * @param response The request response. Cannot be modified in this callback. Instance only
     *         valid within the scope of this method.
     * @return True if |request| was modified or false otherwise
     */
    boolean onResourceResponse(
            CefBrowser browser, CefFrame frame, CefRequest request, CefResponse response);

    /**
     * Called on the IO thread when a resource load has completed. The |browser| and |frame| values
     * represent the source of the request, and may be null for requests originating from service
     * workers or CefURLRequest. This method will be called for all requests, including requests
     * that are aborted due to CEF shutdown or destruction of the associated browser. In cases where
     * the associated browser is destroyed this callback may arrive after the
     * CefLifeSpanHandler.onBeforeClose callback for that browser. The CefFrame.isValid method can
     * be used to test for this situation, and care should be taken not to call |browser| or |frame|
     * methods that modify state (like loadURL, sendProcessMessage, etc.) if the frame is invalid.
     *
     * @param browser The corresponding browser.
     * @param frame The frame generating the event. Instance only valid within the scope of this
     *         method.
     * @param request The request itself. Cannot be modified in this callback. Instance only valid
     *         within the scope of this method.
     * @param response The request response. Cannot be modified in this callback. Instance only
     *         valid within the scope of this method.
     * @param status The load completion status.
     * @param receivedContentLength The number of bytes read.
     */
    void onResourceLoadComplete(CefBrowser browser, CefFrame frame, CefRequest request,
            CefResponse response, CefURLRequest.Status status, long receivedContentLength);

    /**
     * Called on the IO thread to handle requests for URLs with an unknown protocol component. The
     * |browser| and |frame| values represent the source of the request, and may be null for
     * requests originating from service workers or CefURLRequest.
     *
     * SECURITY WARNING: YOU SHOULD USE THIS METHOD TO ENFORCE RESTRICTIONS BASED ON SCHEME, HOST
     * OR OTHER URL ANALYSIS BEFORE ALLOWING OS EXECUTION.
     *
     * @param browser The corresponding browser.
     * @param frame The frame generating the event. Instance only valid within the scope of this
     *         method.
     * @param request The request itself. Cannot be modified in this callback. Instance only valid
     *         within the scope of this method.
     * @param allowOsExecution Set to true to attempt execution via the registered
     * OS protocol handler, if any.
     */
    void onProtocolExecution(
            CefBrowser browser, CefFrame frame, CefRequest request, BoolRef allowOsExecution);
}
