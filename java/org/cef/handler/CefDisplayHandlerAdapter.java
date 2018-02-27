// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.handler;

import org.cef.browser.CefBrowser;
import org.cef.browser.CefFrame;
import org.cef.CefSettings;

/**
 * An abstract adapter class for receiving display events.
 * The methods in this class are empty.
 * This class exists as convenience for creating handler objects.
 */
public abstract class CefDisplayHandlerAdapter implements CefDisplayHandler {
    @Override
    public void onAddressChange(CefBrowser browser, CefFrame frame, String url) {
        return;
    }

    @Override
    public void onTitleChange(CefBrowser browser, String title) {
        return;
    }

    @Override
    public boolean onTooltip(CefBrowser browser, String text) {
        return false;
    }

    @Override
    public void onStatusMessage(CefBrowser browser, String value) {
        return;
    }

    @Override
    public boolean onConsoleMessage(CefBrowser browser, CefSettings.LogSeverity level,
            String message, String source, int line) {
        return false;
    }
}
