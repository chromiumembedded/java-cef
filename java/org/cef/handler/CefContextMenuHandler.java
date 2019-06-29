// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.handler;

import org.cef.browser.CefBrowser;
import org.cef.browser.CefFrame;
import org.cef.callback.CefContextMenuParams;
import org.cef.callback.CefMenuModel;

/**
 * Implement this interface to handle context menu events. The methods of this
 * class will be called on the UI thread.
 */
public interface CefContextMenuHandler {
    /**
     * Called before a context menu is displayed.
     *
     * @param browser The browser generating the event.
     * @param frame The frame generating the event. Instance only valid within the scope
     *      of this method.
     * @param params Provides information about the context menu state. Instance
     *      only valid within the scope of this method.
     * @param model Can be cleared to show no context menu or modified
     *      to show a custom menu. Instance only valid within the scope of this
     *      method.
     */
    public void onBeforeContextMenu(
            CefBrowser browser, CefFrame frame, CefContextMenuParams params, CefMenuModel model);

    /**
     * Called to execute a command selected from the context menu. Return true if
     * the command was handled or false for the default implementation. See
     * cef_menu_id_t for the command ids that have default implementations. All
     * user-defined command ids should be between MENU_ID_USER_FIRST and
     *
     * @param browser The browser generating the event.
     * @param frame The frame generating the event. Instance only valid within the scope
     *      of this method.
     * @param params Will have the same values that were passed to onBeforeContextMenu().
     *      Instance only valid within the scope of this method.
     * @param commandId The id of the command.
     * @param eventFlags A combination of event flags defined in EventFlags
     * @return true if the command was handled or false for the default implementation.
     */
    public boolean onContextMenuCommand(CefBrowser browser, CefFrame frame,
            CefContextMenuParams params, int commandId, int eventFlags);

    /**
     * Called when the context menu is dismissed irregardless of whether the menu
     * was empty or a command was selected.
     *
     * @param browser The browser generating the event.
     * @param frame The corresponding frame. Instance only valid within the scope
     *      of this method.
     */
    public void onContextMenuDismissed(CefBrowser browser, CefFrame frame);
}
