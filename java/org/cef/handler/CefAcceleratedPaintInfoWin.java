// Copyright (c) 2024 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.handler;

/**
 * Windows-specific accelerated paint info.
 */
public class CefAcceleratedPaintInfoWin extends CefAcceleratedPaintInfo {
    /**
     * HANDLE to a texture that can be opened with D3D11 OpenSharedResource.
     */
    public long shared_texture_handle = 0;

    public CefAcceleratedPaintInfoWin() {}

    public CefAcceleratedPaintInfoWin(long shared_texture_handle, int format, int width, int height) {
        super(format, width, height);
        this.shared_texture_handle = shared_texture_handle;
    }

    @Override
    public CefAcceleratedPaintInfoWin clone() {
        return new CefAcceleratedPaintInfoWin(shared_texture_handle, format, width, height);
    }
}
