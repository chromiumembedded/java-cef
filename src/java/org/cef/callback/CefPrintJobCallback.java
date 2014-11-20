// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.callback;

/**
 * Callback interface for asynchronous continuation of print job requests.
 */
public interface CefPrintJobCallback {

  /**
   * Indicate completion of the print job.
   */
  void Continue();
}
