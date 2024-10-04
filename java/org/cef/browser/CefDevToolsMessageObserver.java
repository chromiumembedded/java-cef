// Copyright (c) 2024 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.browser;

/**
 * Used internally by {@link CefDevToolsClient}.
 * <p>
 * Callback interface for {@link CefBrowser#addDevToolsMessageObserver(CefDevToolsMessageObserver)}.
 * The methods of this class will be called on the CEF UI thread.
 */
interface CefDevToolsMessageObserver {
    /**
     * Method that will be called after attempted execution of a DevTools protocol method.
     *
     * @param browser the originating browser instance
     * @param messageId the ID that identifies the originating method call
     * @param success if the method succeeded, |success| will be true and |result| will be a JSON
     *        object containing the method call result. If the method call failed, |success| will
     *        be false and |result| will be a JSON object describing the error.
     * @param result method call result or an error
     */
    void onDevToolsMethodResult(CefBrowser browser, int messageId, boolean success, String result);

    /**
     * Method that will be called on receipt of a DevTools protocol event.
     *
     * @param browser the originating browser instance
     * @param method the method name
     * @param parameters the event data
     */
    void onDevToolsEvent(CefBrowser browser, String method, String parameters);
}
