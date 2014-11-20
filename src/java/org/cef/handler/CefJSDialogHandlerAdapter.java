// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.handler;

import org.cef.browser.CefBrowser;
import org.cef.callback.CefJSDialogCallback;
import org.cef.misc.BoolRef;

/**
 * An abstract adapter class for receiving javascript dialog requests.
 * The methods in this class are empty.
 * This class exists as convenience for creating handler objects.
 */
public abstract class CefJSDialogHandlerAdapter implements CefJSDialogHandler {
  @Override
  public boolean onJSDialog(CefBrowser browser, String origin_url,
      String accept_lang, JSDialogType dialog_type, String message_text,
      String default_prompt_text, CefJSDialogCallback callback,
      BoolRef suppress_message) {
    return false;
  }

  @Override
  public boolean onBeforeUnloadDialog(CefBrowser browser, String message_text,
      boolean is_reload, CefJSDialogCallback callback) {
    return false;
  }

  @Override
  public void onResetDialogState(CefBrowser browser) {
  }

  @Override
  public void onDialogClosed(CefBrowser browser) {
  }
}
