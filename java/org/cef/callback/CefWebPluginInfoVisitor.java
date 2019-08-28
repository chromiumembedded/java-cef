// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.callback;

import org.cef.network.CefWebPluginInfo;

/**
 * Interface to implement for visiting web plugin information. The methods of this class will be
 * called on the browser process UI thread.
 */
public interface CefWebPluginInfoVisitor {
    /**
     * Method that will be called once for each plugin. This method may never be called if no
     * plugins are found.
     *
     * @param info Information for the current plugin. Instance only valid within the scope of this
     *         method.
     * @param count The 0-based index for the current plugin.
     * @param total The total number of plugins.
     * @return True to continue visiting plugins or false to stop.
     */
    public boolean visit(CefWebPluginInfo info, int count, int total);
}
