// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.callback;

/**
 * Interface to implement for receiving unstable plugin information. The methods
 * of this class will be called on the browser process IO thread.
 */
public interface CefWebPluginUnstableCallback {

  /**
   * Method that will be called for the requested plugin. |unstable| will be
   * true if the plugin has reached the crash count threshold of 3 times in 120
   * seconds.
   */
  public void isUnstable(String path, boolean unstable);
}
