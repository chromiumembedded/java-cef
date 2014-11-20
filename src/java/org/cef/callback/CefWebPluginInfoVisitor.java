// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.callback;

import org.cef.network.CefWebPluginInfo;

/**
 * Interface to implement for visiting web plugin information. The methods of
 * this class will be called on the browser process UI thread.
 */
public interface CefWebPluginInfoVisitor {

  /**
   * Method that will be called once for each plugin. |count| is the 0-based
   * index for the current plugin. |total| is the total number of plugins.
   * Return false to stop visiting plugins. This method may never be called if
   * no plugins are found.
   */
  public boolean visit(CefWebPluginInfo info, int count, int total);
}
