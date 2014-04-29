// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package tests.detailed.handler;

import java.awt.Frame;

import org.cef.browser.CefBrowser;
import org.cef.callback.CefContextMenuParams;
import org.cef.callback.CefMenuModel;
import org.cef.callback.CefMenuModel.MenuId;
import org.cef.handler.CefContextMenuHandlerAdapter;

import tests.detailed.dialog.SearchDialog;
import tests.detailed.dialog.ShowTextDialog;

public class ContextMenuHandler extends CefContextMenuHandlerAdapter {
  private final Frame owner_;

  public ContextMenuHandler(Frame owner) {
    owner_ = owner;
  }

  @Override
  public void onBeforeContextMenu(CefBrowser browser,
                                  CefContextMenuParams params,
                                  CefMenuModel model) {
    model.addItem(CefMenuModel.MenuId.MENU_ID_FIND, "Find...");
    if (params.hasImageContents())
      model.addItem(CefMenuModel.MenuId.MENU_ID_USER_FIRST, "Download Image...");
  }
  @Override
  public boolean onContextMenuCommand(CefBrowser browser,
                                      CefContextMenuParams params,
                                      int commandId,
                                      int eventFlags) {
    switch (commandId) {
      case MenuId.MENU_ID_VIEW_SOURCE:
        ShowTextDialog visitor = new ShowTextDialog(owner_, "Source of \"" + browser.getURL() + "\"");
        browser.getSource(visitor);
        return true;
      case MenuId.MENU_ID_FIND:
        SearchDialog search = new SearchDialog(owner_, browser);
        search.setVisible(true);
        return true;
      case MenuId.MENU_ID_USER_FIRST:
        browser.startDownload(params.getSourceUrl());
        return true;
      default:
        return false;
    }
  }
}
