// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.handler;

import org.cef.browser.CefBrowser;
import org.cef.callback.CefContextMenuParams;
import org.cef.callback.CefMenuModel;

/**
 * An abstract adapter class for receiving context menu events. 
 * The methods in this class are empty.
 * This class exists as convenience for creating handler objects.
 */
public abstract class CefContextMenuHandlerAdapter implements CefContextMenuHandler {
  @Override
  public void onBeforeContextMenu(CefBrowser browser,
                                  CefContextMenuParams params,
                                  CefMenuModel model) {
  }

  @Override
  public boolean onContextMenuCommand(CefBrowser browser,
                                      CefContextMenuParams params,
                                      int commandId,
                                      int eventFlags) {
    return false;
  }

  @Override
  public void onContextMenuDismissed(CefBrowser browser) {
  }
}
