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
   * @param title to be used for the dialog and may be empty to show the default title ("Open" or "Save" depending on the mode).
   * @param defaultFileName is the default file name to select in the dialog.
   * @param acceptTypes is a list of valid lower-cased MIME types or file extensions
   * specified in an input element and is used to restrict selectable files to such types.
   * @param callback is a callback handler for handling own file dialogs.
   * 
   * @return To display a custom dialog return true and execute callback.
   * To display the default dialog return false.
   */
  public boolean onFileDialog(CefBrowser browser,
                              FileDialogMode mode,
                              String title,
                              String defaultFileName,
                              Vector<String> acceptTypes,
                              CefFileDialogCallback callback);
}
