// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.handler;

import org.cef.browser.CefBrowser;
import org.cef.callback.CefJSDialogCallback;
import org.cef.misc.BoolRef;

/**
 * Implement this interface to handle events related to JavaScript dialogs. The
 * methods of this class will be called on the UI thread.
 */
public interface CefJSDialogHandler {
  /**
   * Supported JavaScript dialog types.
   */
  public static enum JSDialogType {
    JSDIALOGTYPE_ALERT,
    JSDIALOGTYPE_CONFIRM,
    JSDIALOGTYPE_PROMPT,
  }

  /**
   * Called to run a JavaScript dialog. Set suppress_message to true and
   * return false to suppress the message (suppressing messages is preferable
   * to immediately executing the callback as this is used to detect presumably
   * malicious behavior like spamming alert messages in onbeforeunload). Set
   * suppress_message to false and return false to use the default
   * implementation (the default implementation will show one modal dialog at a
   * time and suppress any additional dialog requests until the displayed dialog
   * is dismissed). Return true if the application will use a custom dialog or
   * if the callback has been executed immediately. Custom dialogs may be either
   * modal or modeless. If a custom dialog is used the application must execute
   * callback once the custom dialog is dismissed.
   * 
   * @param browser The corresponding browser.
   * @param origin_url The originating url.
   * @param accept_lang the accepted language.
   * @param dialog_type the dialog type.
   * @param message_text the text to be displayed.
   * @param default_prompt_text value will be specified for prompt dialogs only.
   * @param callback execute callback once the custom dialog is dismissed.
   * @param suppress_message set to true to suppress displaying the message. 
   * @return false to use the default dialog implementation. Return true if the
   * application will use a custom dialog.
   */
  public boolean onJSDialog(CefBrowser browser,
                            String origin_url,
                            String accept_lang,
                            JSDialogType dialog_type,
                            String message_text,
                            String default_prompt_text,
                            CefJSDialogCallback callback,
                            BoolRef suppress_message);

  /**
   * Called to run a dialog asking the user if they want to leave a page. Return
   * false to use the default dialog implementation. Return true if the
   * application will use a custom dialog or if the callback has been executed
   * immediately. Custom dialogs may be either modal or modeless. If a custom
   * dialog is used the application must execute callback once the custom
   * dialog is dismissed.
   * 
   * @param browser The corresponding browser.
   * @param message_text The text to be displayed.
   * @param is_reload true if the page is reloaded.
   * @param callback execute callback once the custom dialog is dismissed.
   * @return false to use the default dialog implementation. Return true if the
   * application will use a custom dialog.
   */
  public boolean onBeforeUnloadDialog(CefBrowser browser,
                                      String message_text,
                                      boolean is_reload,
                                      CefJSDialogCallback callback);

  /**
   * Called to cancel any pending dialogs and reset any saved dialog state. Will
   * be called due to events like page navigation irregardless of whether any
   * dialogs are currently pending.
   */
  public void onResetDialogState(CefBrowser browser);

  /**
   * Called when the default implementation dialog is closed.
   */
  public void onDialogClosed(CefBrowser browser);
}
