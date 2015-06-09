// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.callback;

/**
 * Callback interface used for asynchronous continuation of authentication
 * requests.
 */
public interface CefAuthCallback {
  /**
   * Continue the authentication request.
   */
  public void Continue(String username, String password);

  /**
   * Cancel the authentication request.
   */
  public void cancel();
}
