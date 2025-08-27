// Copyright (c) 2024 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.handler;

/**
 * Structure representing shared texture info for accelerated painting.
 */
public class CefAcceleratedPaintInfo {
    /**
     * Shared texture handle. The meaning depends on the platform:
     * - Windows: HANDLE to a texture that can be opened with D3D11 OpenSharedResource
     * - macOS: IOSurface pointer that can be opened with Metal or OpenGL
     * - Linux: Contains several planes, each with an fd to the underlying system native buffer
     */
    public long shared_texture_handle = 0;
    
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
    
    public CefAcceleratedPaintInfo(long shared_texture_handle, int format, int width, int height) {
        this.shared_texture_handle = shared_texture_handle;
        this.format = format;
        this.width = width;
        this.height = height;
    }
    
    @Override
    public CefAcceleratedPaintInfo clone() {
        return new CefAcceleratedPaintInfo(shared_texture_handle, format, width, height);
    }
}
