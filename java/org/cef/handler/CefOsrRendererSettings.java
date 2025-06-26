// Copyright (c) 2024 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.handler;

/**
 * Settings for off-screen rendering (OSR) configuration.
 * Based on CEF's OsrRendererSettings structure.
 */
public class CefOsrRendererSettings {
    
    /**
     * If true draw a border around update rectangles.
     */
    public boolean show_update_rect = false;

    /**
     * If true return real screen bounds from GetRootScreenRect/GetScreenInfo.
     * - Allows window.outerWidth/Height and window.screenX/Y to return correct values.
     * - Allows JavaScript window.moveTo/By() and window.resizeTo/By() to provide
     *   bounds that include the window frame.
     * - Causes HTML select popups to be cropped (limitation of implementation).
     */
    public boolean real_screen_bounds = true;

    /**
     * Background color. Enables transparency if the alpha component is 0.
     * 32-bit ARGB color value, not premultiplied.
     */
    public int background_color = 0;

    /**
     * Render using shared textures. Supported on Windows only via D3D11.
     * When enabled, the browser will render to a shared texture that can be 
     * accessed by the host application for hardware-accelerated compositing.
     */
    public boolean shared_texture_enabled = false;

    /**
     * Client implements a BeginFrame timer by calling
     * CefBrowserHost::SendExternalBeginFrame at the specified frame rate.
     */
    public boolean external_begin_frame_enabled = false;
    
    /**
     * Frame rate for external begin frame when external_begin_frame_enabled is true.
     */
    public int begin_frame_rate = 0;

    public CefOsrRendererSettings() {}

    /**
     * Copy constructor.
     */
    public CefOsrRendererSettings(CefOsrRendererSettings other) {
        this.show_update_rect = other.show_update_rect;
        this.real_screen_bounds = other.real_screen_bounds;
        this.background_color = other.background_color;
        this.shared_texture_enabled = other.shared_texture_enabled;
        this.external_begin_frame_enabled = other.external_begin_frame_enabled;
        this.begin_frame_rate = other.begin_frame_rate;
    }

    @Override
    public CefOsrRendererSettings clone() {
        return new CefOsrRendererSettings(this);
    }
}
