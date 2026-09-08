// Copyright (c) 2024 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.handler;

/**
 * Linux-specific accelerated paint info (dmabuf planes).
 */
public class CefAcceleratedPaintInfoLinux extends CefAcceleratedPaintInfo {
    /**
     * dmabuf plane count.
     */
    public int plane_count = 0;

    /**
     * dmabuf plane file descriptors.
     */
    public int[] plane_fds = null;

    /**
     * dmabuf plane strides (bytes per row).
     */
    public int[] plane_strides = null;

    /**
     * dmabuf plane offsets.
     */
    public long[] plane_offsets = null;

    /**
     * dmabuf plane sizes.
     */
    public long[] plane_sizes = null;

    /**
     * dmabuf modifier.
     */
    public long modifier = 0;

    public CefAcceleratedPaintInfoLinux() {}

    public CefAcceleratedPaintInfoLinux(int format, int width, int height) {
        super(format, width, height);
    }

    public boolean hasDmaBufPlanes() {
        return plane_count > 0 && plane_fds != null && plane_strides != null && plane_offsets != null;
    }

    @Override
    public CefAcceleratedPaintInfoLinux clone() {
        CefAcceleratedPaintInfoLinux clone = new CefAcceleratedPaintInfoLinux(format, width, height);
        clone.plane_count = plane_count;
        clone.modifier = modifier;
        clone.plane_fds = plane_fds != null ? plane_fds.clone() : null;
        clone.plane_strides = plane_strides != null ? plane_strides.clone() : null;
        clone.plane_offsets = plane_offsets != null ? plane_offsets.clone() : null;
        clone.plane_sizes = plane_sizes != null ? plane_sizes.clone() : null;
        return clone;
    }
}
