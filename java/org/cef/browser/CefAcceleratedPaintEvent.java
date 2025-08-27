// Copyright (c) 2024 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.browser;

import org.cef.handler.CefAcceleratedPaintInfo;

import java.awt.*;

public class CefAcceleratedPaintEvent {
    private final CefBrowser browser;
    private final boolean popup;
    private final Rectangle[] dirtyRects;
    private final CefAcceleratedPaintInfo acceleratedPaintInfo;

    public CefAcceleratedPaintEvent(CefBrowser browser, boolean popup, Rectangle[] dirtyRects,
            CefAcceleratedPaintInfo acceleratedPaintInfo) {
        this.browser = browser;
        this.popup = popup;
        this.dirtyRects = dirtyRects;
        this.acceleratedPaintInfo = acceleratedPaintInfo;
    }

    public CefBrowser getBrowser() {
        return browser;
    }

    public boolean getPopup() {
        return popup;
    }

    public Rectangle[] getDirtyRects() {
        return dirtyRects;
    }

    public CefAcceleratedPaintInfo getAcceleratedPaintInfo() {
        return acceleratedPaintInfo;
    }

    public int getWidth() {
        return acceleratedPaintInfo.width;
    }

    public int getHeight() {
        return acceleratedPaintInfo.height;
    }
}
