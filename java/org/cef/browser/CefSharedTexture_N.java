// Copyright (c) 2026 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.browser;

/**
 * Binds CEF shared textures to the GL_TEXTURE_2D bound in the current OpenGL context. JOGL
 * exposes neither EXT_memory_object_win32 nor the DRI3 request needed for dmabufs on GLX.
 */
final class CefSharedTexture_N {
    private CefSharedTexture_N() {}

    /**
     * Windows only. Imports a D3D11 shared texture handle through EXT_memory_object_win32.
     *
     * @return 0 on success, -1 if the extension is unavailable, otherwise the GL error.
     */
    static native int N_BindD3D11Texture(long handle, int width, int height);

    /**
     * Linux only. Wraps the dmabuf planes in a DRI3 pixmap and binds it through
     * GLX_EXT_texture_from_pixmap.
     *
     * @return a token for {@link #N_ReleaseDmaBuf}, or 0 on failure.
     */
    static native long N_BindDmaBuf(int width, int height, int planeCount, int[] fds, int[] strides,
            long[] offsets, long modifier);

    static native void N_ReleaseDmaBuf(long token);
}
