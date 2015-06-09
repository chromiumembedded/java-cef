// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.handler;

import org.cef.browser.CefBrowser;
import org.cef.callback.CefBeforeDownloadCallback;
import org.cef.callback.CefDownloadItem;
import org.cef.callback.CefDownloadItemCallback;

/**
 * Implement this interface to handle file downloads. The methods of this class
 * will called on the browser process UI thread.
 */
public interface CefDownloadHandler {
  /**
   * Called before a download begins. By default the download will be canceled.
   * Execute callback either asynchronously or in this method to continue the download
   * if desired.
   * 
   * @param browser The desired browser.
   * @param downloadItem The item to be downloaded. Do not keep a reference to it outside this method.
   * @param suggestedName is the suggested name for the download file.
   * @param callback start the download by calling the Continue method
   */
  public void onBeforeDownload(CefBrowser browser,
                               CefDownloadItem downloadItem,
                               String suggestedName,
                               CefBeforeDownloadCallback callback);

  /**
   * Called when a download's status or progress information has been updated.
   * @param browser The desired browser.
   * @param downloadItem The downloading item.
   * @param callback Execute callback to cancel the download
   */
  public void onDownloadUpdated(CefBrowser browser,
                                CefDownloadItem downloadItem,
                                CefDownloadItemCallback callback);
}
