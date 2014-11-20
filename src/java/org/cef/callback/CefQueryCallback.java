// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.callback;

/**
 * Interface representing a query callback.
 */
public interface CefQueryCallback {
  /**
   * Notify the associated JavaScript onSuccess callback that the query has
   * completed successfully.
   * @param response Response passed to JavaScript.
   */
  public void success(String response);
  
  /**
   * Notify the associated JavaScript onFailure callback that the query has
   * failed.
   * @param error_code Error code passed to JavaScript.
   * @param error_message Error message passed to JavaScript.
   */
  public void failure(int error_code, String error_message);
}
