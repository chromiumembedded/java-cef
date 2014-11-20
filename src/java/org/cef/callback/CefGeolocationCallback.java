// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.callback;

/**
 * Callback interface used for asynchronous continuation of geolocation
 * permission requests.
 */
public interface CefGeolocationCallback {
  /**
   * Call to allow or deny geolocation access.
   */
  public void Continue(boolean allow);
}
