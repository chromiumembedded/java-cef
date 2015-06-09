// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.handler;

import org.cef.browser.CefBrowser;
import org.cef.callback.CefContextMenuParams;
import org.cef.callback.CefMenuModel;

/**
 * Implement this interface to handle context menu events. The methods of this
 * class will be called on the UI thread.
 */
public interface CefContextMenuHandler {

  /**
   * Supported event bit flags.
   */
  public static final class EventFlags {
    public final static int EVENTFLAG_NONE                = 0;
    public final static int EVENTFLAG_CAPS_LOCK_ON        = 1 << 0;
    public final static int EVENTFLAG_SHIFT_DOWN          = 1 << 1;
    public final static int EVENTFLAG_CONTROL_DOWN        = 1 << 2;
    public final static int EVENTFLAG_ALT_DOWN            = 1 << 3;
    public final static int EVENTFLAG_LEFT_MOUSE_BUTTON   = 1 << 4;
    public final static int EVENTFLAG_MIDDLE_MOUSE_BUTTON = 1 << 5;
    public final static int EVENTFLAG_RIGHT_MOUSE_BUTTON  = 1 << 6;
    // Mac OS-X command key.
    public final static int EVENTFLAG_COMMAND_DOWN        = 1 << 7;
    public final static int EVENTFLAG_NUM_LOCK_ON         = 1 << 8;
    public final static int EVENTFLAG_IS_KEY_PAD          = 1 << 9;
    public final static int EVENTFLAG_IS_LEFT             = 1 << 10;
    public final static int EVENTFLAG_IS_RIGHT            = 1 << 11;
  }

  /**
   * Called before a context menu is displayed.
   * 
   * @param browser The corresponding browser.
   * @param params provides information about the context menu state.
   * @param model can be cleared to show no context menu or modified
   * to show a custom menu.
   */
  public void onBeforeContextMenu(CefBrowser browser,
                                  CefContextMenuParams params,
                                  CefMenuModel model);

  /**
   * Called to execute a command selected from the context menu. Return true if
   * the command was handled or false for the default implementation. See
   * cef_menu_id_t for the command ids that have default implementations. All
   * user-defined command ids should be between MENU_ID_USER_FIRST and
   * 
   * @param browser The corresponding browser.
   * @param params Will have the same values as what was passed to onBeforeContextMenu().
   * @param commandId The id of the command.
   * @param eventFlags A combination of event flags defined in EventFlags
   * @return true if the command was handled or false for the default implementation.
   */
  public boolean onContextMenuCommand(CefBrowser browser,
                                      CefContextMenuParams params,
                                      int commandId,
                                      int eventFlags);

  /**
   * Called when the context menu is dismissed irregardless of whether the menu
   * was empty or a command was selected.
   * 
   * @param browser The corresponding browser.
   */
  public void onContextMenuDismissed(CefBrowser browser);
}
