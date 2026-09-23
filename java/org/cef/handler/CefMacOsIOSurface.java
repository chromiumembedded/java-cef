// Copyright (c) 2026 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.handler;

/**
 * macOS IOSurface helpers used by accelerated off-screen rendering.
 */
public final class CefMacOsIOSurface {
    private CefMacOsIOSurface() {}

    /**
     * Binds the IOSurface to the currently bound GL_TEXTURE_RECTANGLE texture in
     * the current CGL context.
     *
     * @return CGLError, where 0 means success.
     */
    public static native int bindToCurrentTexture(long ioSurface, int width, int height);
}
