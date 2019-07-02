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
 * An abstract adapter class for cookie access filter events.
 * The methods in this class are empty.
 * This class exists as convenience for creating handler objects.
 */
public abstract class CefCookieAccessFilterAdapter implements CefCookieAccessFilter {
    @Override
    public boolean canSendCookie(
            CefBrowser browser, CefFrame frame, CefRequest request, CefCookie cookie) {
        return true;
    }

    @Override
    public boolean canSaveCookie(CefBrowser browser, CefFrame frame, CefRequest request,
            CefResponse response, CefCookie cookie) {
        return true;
    }
}
