// Copyright (c) 2024 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.handler;

/**
 * Base structure representing accelerated paint info. Platform-specific
 * details are provided by subclasses.
 */
public class CefAcceleratedPaintInfo implements Cloneable {
    /**
     * Format of the shared texture.
     */
    public int format = 0;

    /**
     * Size information for the shared texture.
     */
    public int width = 0;
    public int height = 0;

    public CefAcceleratedPaintInfo() {}

    protected CefAcceleratedPaintInfo(int format, int width, int height) {
        this.format = format;
        this.width = width;
        this.height = height;
    }

    @Override
    public CefAcceleratedPaintInfo clone() {
        return new CefAcceleratedPaintInfo(format, width, height);
    }
}
