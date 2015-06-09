// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.handler;

import java.util.Vector;

import org.cef.browser.CefBrowser;
import org.cef.callback.CefFileDialogCallback;

/**
 * Implement this interface to handle dialog events. The methods of this class
 * will be called on the browser process UI thread.
 */
public interface CefDialogHandler {

  /**
   * Supported file dialog modes.
   */
  enum FileDialogMode {
    FILE_DIALOG_OPEN, //!< Requires that the file exists before allowing the user to pick it.
    FILE_DIALOG_OPEN_MULTIPLE, //!< Like Open, but allows picking multiple files to open.
    FILE_DIALOG_SAVE //!< Allows picking a nonexistent file, and prompts to overwrite if the file already exists.
  }

  /**
   * Called to run a file chooser dialog.
   * 
   * @param browser
   * @param mode represents the type of dialog to display.
   * @param title to be used for the dialog and may be empty to show the default
   * title ("Open" or "Save" depending on the mode).
   * @param defaultFilePath is the path with optional directory and/or file name
   * component that should be initially selected in the dialog.
   * @param acceptFilters are used to restrict the selectable file types and may
   * any combination of (a) valid lower-cased MIME types (e.g. "text/*" or
   * "image/*"), (b) individual file extensions (e.g. ".txt" or ".png"), or (c)
   * combined description and file extension delimited using "|" and ";" (e.g.
   * "Image Types|.png;.gif;.jpg").
   * @param selectedAcceptFilter is the 0-based index of the filter that should
   * be selected by default.
   * @param callback is a callback handler for handling own file dialogs.
   * 
   * @return To display a custom dialog return true and execute callback.
   * To display the default dialog return false.
   */
  public boolean onFileDialog(CefBrowser browser,
                              FileDialogMode mode,
                              String title,
                              String defaultFilePath,
                              Vector<String> acceptFilters,
                              int selectedAcceptFilter,
                              CefFileDialogCallback callback);
}
