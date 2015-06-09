// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.callback;

/**
 * Callback interface used for asynchronous continuation of JavaScript dialog
 * requests.
 */
public interface CefJSDialogCallback {
  /**
   * Continue the JS dialog request.
   * 
   * @param success Set to true if the OK button was pressed.
   * @param user_input The value should be specified for prompt dialogs.
   */
  public void Continue(boolean success, String user_input);
}
