// Copyright (c) 2015 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.handler;

import org.cef.browser.CefBrowser;

import java.awt.Rectangle;

/**
 * Implement this interface to handle events if window rendering is enabled.
 *
 */
public interface CefWindowHandler {
    /**
     * Retrieve the view rectangle.
     * @param browser The browser generating the event.
     * @return The view rectangle.
     */
    public Rectangle getRect(CefBrowser browser);

    /**
     * Implement this method to handle mouse events on Windows.
     *
     * The method is called in case of the following events:
     * MOUSE_MOVED, MOUSE_PRESSED, MOUSE_RELEASED and MOUSE_WHEEL for a
     * horizontal wheel movement.
     *
     * @param browser The browser generating the event.
     * @param event A mouse event like MouseEvent.MOUSE_MOVED.
     * @param screenX The absolute X position on the screen.
     * @param screenY The absolute Y position on the screen.
     * @param modifier The modifier keys down during event (e.g. Shift, Ctrl).
     * @param button An integer indicating which mouse button state changed.
     * If parameter event is set to MouseEvent.MOUSE_WHEEL, the value of this
     * parameter specifies the rotation indicator (negative value for left scroll
     * and positive value for right scroll).
     */
    public void onMouseEvent(
            CefBrowser browser, int event, int screenX, int screenY, int modifier, int button);
}
