// Copyright (c) 2024 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.handler;

/**
 * macOS-specific accelerated paint info.
 */
public class CefAcceleratedPaintInfoMac extends CefAcceleratedPaintInfo {
    /**
     * IOSurface handle that can be opened with Metal or OpenGL.
     */
    public long shared_texture_io_surface = 0;

    public CefAcceleratedPaintInfoMac() {}

    public CefAcceleratedPaintInfoMac(long shared_texture_io_surface, int format, int width, int height) {
        super(format, width, height);
        this.shared_texture_io_surface = shared_texture_io_surface;
    }

    @Override
    public CefAcceleratedPaintInfoMac clone() {
        return new CefAcceleratedPaintInfoMac(shared_texture_io_surface, format, width, height);
    }
}
