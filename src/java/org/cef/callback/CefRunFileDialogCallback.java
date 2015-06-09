// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.callback;

import java.util.Vector;
import org.cef.browser.CefBrowser;

/**
 * Callback interface for CefBrowserHost::RunFileDialog. The methods of this
 * class will be called on the browser process UI thread.
 */
public interface CefRunFileDialogCallback {

  /**
   * Called asynchronously after the file dialog is dismissed. If the selection
   * was successful filePaths will be a single value or a list of values
   * depending on the dialog mode. If the selection was cancelled filePaths
   * will be empty.
   * 
   * @param selectedAcceptFilter 0-based index of the value selected from
   * the accept filters array passed to CefBrowserHost::RunFileDialog.
   * @param filePaths list of file paths or empty list.
   */
  void onFileDialogDismissed(int selectedAcceptFilter, Vector<String> filePaths);
}
