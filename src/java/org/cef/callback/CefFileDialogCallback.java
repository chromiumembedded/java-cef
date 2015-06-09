// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.callback;

import java.util.Vector;

/**
 * Callback interface for asynchronous continuation of file dialog requests.
 */
public interface CefFileDialogCallback {

  /**
   * Continue the file selection with the specified file_paths. This may be
   * a single value or a list of values depending on the dialog mode. An empty
   * value is treated the same as calling Cancel().
   * 
   * @param selectedAcceptFilter 0-based index of the value selected from the
   * accept filters array passed to CefDialogHandler::OnFileDialog.
   * @param filePaths list of selected file paths or an empty list.
   */
  public void Continue(int selectedAcceptFilter, Vector<String> filePaths);

  /**
   * Cancel the file selection.
   */
  public void Cancel();
}
