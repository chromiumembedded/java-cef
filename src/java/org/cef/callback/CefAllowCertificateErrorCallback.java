// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.callback;

/**
 * Callback interface used for asynchronous continuation of url requests when
 * invalid SSL certificates are encountered.
 */
public interface CefAllowCertificateErrorCallback {
  /**
   * Continue the url request.
   * 
   * @param allow If set to true the request will be allowed.
   *   Otherwise, the request will be denied.
   */
  void Continue(boolean allow);
}
