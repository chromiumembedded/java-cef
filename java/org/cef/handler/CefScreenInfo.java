// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.handler;

import java.awt.Rectangle;

/**
 *
 * @author shannah
 */
public class CefScreenInfo {
    public double device_scale_factor;
    public int depth;
    public int depth_per_component;
    public boolean is_monochrome;
    public int x, y, width, height;
    public int available_x, available_y, available_width, available_height;

    public void Set(double device_scale_factor, int depth, int depth_per_component,
            boolean is_monochrome, Rectangle rect, Rectangle availableRect) {
        this.device_scale_factor = device_scale_factor;
        this.depth = depth;
        this.depth_per_component = depth_per_component;
        this.is_monochrome = is_monochrome;
        this.x = rect.x;
        this.y = rect.y;
        this.width = rect.width;
        this.height = rect.height;
        this.available_x = availableRect.x;
        this.available_y = availableRect.y;
        this.available_width = availableRect.width;
        this.available_height = availableRect.height;
    }
}
