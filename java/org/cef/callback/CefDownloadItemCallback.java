// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.callback;

/**
 * Callback interface used to asynchronously cancel a download.
 */
public interface CefDownloadItemCallback {
  /**
   * Call to cancel the download.
   */
  public void cancel();
}
