// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.callback;

import org.cef.misc.BoolRef;
import org.cef.misc.IntRef;

/**
 * Supports creation and modification of menus. See cef_menu_id_t for the
 * command ids that have default implementations. All user-defined command ids
 * should be between MENU_ID_USER_FIRST and MENU_ID_USER_LAST. The methods of
 * this class can only be accessed on the browser process the UI thread.
 */
public interface CefMenuModel {

  public static final class MenuId {
    public static final int MENU_ID_BACK                = 100;
    public static final int MENU_ID_FORWARD             = 101;
    public static final int MENU_ID_RELOAD              = 102;
    public static final int MENU_ID_RELOAD_NOCACHE      = 103;
    public static final int MENU_ID_STOPLOAD            = 104;

    // Editing.
    public static final int MENU_ID_UNDO                = 110;
    public static final int MENU_ID_REDO                = 111;
    public static final int MENU_ID_CUT                 = 112;
    public static final int MENU_ID_COPY                = 113;
    public static final int MENU_ID_PASTE               = 114;
    public static final int MENU_ID_DELETE              = 115;
    public static final int MENU_ID_SELECT_ALL          = 116;

    // Miscellaneous.
    public static final int MENU_ID_FIND                = 130;
    public static final int MENU_ID_PRINT               = 131;
    public static final int MENU_ID_VIEW_SOURCE         = 132;

    // Spell checking word correction suggestions.
    public static final int MENU_ID_SPELLCHECK_SUGGESTION_0     = 200;
    public static final int MENU_ID_SPELLCHECK_SUGGESTION_1     = 201;
    public static final int MENU_ID_SPELLCHECK_SUGGESTION_2     = 202;
    public static final int MENU_ID_SPELLCHECK_SUGGESTION_3     = 203;
    public static final int MENU_ID_SPELLCHECK_SUGGESTION_4     = 204;
    public static final int MENU_ID_SPELLCHECK_SUGGESTION_LAST  = 204;
    public static final int MENU_ID_NO_SPELLING_SUGGESTIONS     = 205;

    // All user-defined menu IDs should come between MENU_ID_USER_FIRST and
    // MENU_ID_USER_LAST to avoid overlapping the Chromium and CEF ID ranges
    // defined in the tools/gritsettings/resource_ids file.
    public static final int MENU_ID_USER_FIRST          = 26500;
    public static final int MENU_ID_USER_LAST           = 28500;
  }

  /**
   * Supported menu item types.
   */
  public enum MenuItemType {
    MENUITEMTYPE_NONE,
    MENUITEMTYPE_COMMAND,
    MENUITEMTYPE_CHECK,
    MENUITEMTYPE_RADIO,
    MENUITEMTYPE_SEPARATOR,
    MENUITEMTYPE_SUBMENU,
  }

  /**
   * Clears the menu. Returns true on success.
   */
  boolean clear();

  /**
   * Returns the number of items in this menu.
   */
  int getCount();

  /**
   * Add a separator to the menu. Returns true on success.
   */
  boolean addSeparator();

  /**
   * Add an item to the menu. Returns true on success.
   */
  boolean addItem(int command_id, String label);

  /**
   * Add a check item to the menu. Returns true on success.
   */
  boolean addCheckItem(int command_id, String label);

  /**
   * Add a radio item to the menu. Only a single item with the specified
   * |group_id| can be checked at a time. Returns true on success.
   */
  boolean addRadioItem(int command_id, String label, int group_id);

  /**
   * Add a sub-menu to the menu. The new sub-menu is returned.
   */
  CefMenuModel addSubMenu(int command_id, String label);

  /**
   * Insert a separator in the menu at the specified |index|. Returns true on
   * success.
   */
  boolean insertSeparatorAt(int index);

  /**
   * Insert an item in the menu at the specified |index|. Returns true on
   * success.
   */
  boolean insertItemAt(int index, int command_id, String label);

  /**
   * Insert a check item in the menu at the specified |index|. Returns true on
   * success.
   */
  boolean insertCheckItemAt(int index, int command_id, String label);

  /**
   * Insert a radio item in the menu at the specified |index|. Only a single
   * item with the specified |group_id| can be checked at a time. Returns true
   * on success.
   */
  boolean insertRadioItemAt(int index, int command_id, String label, int group_id);

  /**
   * Insert a sub-menu in the menu at the specified |index|. The new sub-menu
   * is returned.
   */
  CefMenuModel insertSubMenuAt(int index, int command_id, String label);

  /**
   * Removes the item with the specified |command_id|. Returns true on success.
   */
  boolean remove(int command_id);

  /**
   * Removes the item at the specified |index|. Returns true on success.
   */
  boolean removeAt(int index);

  /**
   * Returns the index associated with the specified |command_id| or -1 if not
   * found due to the command id not existing in the menu.
   */
  int getIndexOf(int command_id);

  /**
   * Returns the command id at the specified |index| or -1 if not found due to
   * invalid range or the index being a separator.
   */
  int getCommandIdAt(int index);

  /**
   * Sets the command id at the specified |index|. Returns true on success.
   */
  boolean setCommandIdAt(int index, int command_id);

  /**
   * Returns the label for the specified |command_id| or empty if not found.
   */
  String getLabel(int command_id);

  /**
   * Returns the label at the specified |index| or empty if not found due to
   * invalid range or the index being a separator.
   */
  String getLabelAt(int index);

  /**
   * Sets the label for the specified |command_id|. Returns true on success.
   */
  boolean setLabel(int command_id, String label);

  /**
   * Set the label at the specified |index|. Returns true on success.
   */
  boolean setLabelAt(int index, String label);

  /**
   * Returns the item type for the specified |command_id|.
   */
  MenuItemType getType(int command_id);

  /**
   * Returns the item type at the specified |index|.
   */
  MenuItemType getTypeAt(int index);

  /**
   * Returns the group id for the specified |command_id| or -1 if invalid.
   */
  int getGroupId(int command_id);

  /**
   * Returns the group id at the specified |index| or -1 if invalid.
   */
  int getGroupIdAt(int index);

  /**
   * Sets the group id for the specified |command_id|. Returns true on success.
   */
  boolean setGroupId(int command_id, int group_id);

  /**
   * Sets the group id at the specified |index|. Returns true on success.
   */
  boolean setGroupIdAt(int index, int group_id);

  /**
   * Returns the submenu for the specified |command_id| or empty if invalid.
   */
  CefMenuModel getSubMenu(int command_id);

  /**
   * Returns the submenu at the specified |index| or empty if invalid.
   */
  CefMenuModel getSubMenuAt(int index);

  /**
   * Returns true if the specified |command_id| is visible.
   */
  boolean isVisible(int command_id);

  /**
   * Returns true if the specified |index| is visible.
   */
  boolean isVisibleAt(int index);

  /**
   * Change the visibility of the specified |command_id|. Returns true on
   * success.
   */
  boolean setVisible(int command_id, boolean visible);

  /**
   * Change the visibility at the specified |index|. Returns true on success.
   */
  boolean setVisibleAt(int index, boolean visible);

  /**
   * Returns true if the specified |command_id| is enabled.
   */
  boolean isEnabled(int command_id);

  /**
   * Returns true if the specified |index| is enabled.
   */
  boolean isEnabledAt(int index);

  /**
   * Change the enabled status of the specified |command_id|. Returns true on
   * success.
   */
  boolean setEnabled(int command_id, boolean enabled);

  /**
   * Change the enabled status at the specified |index|. Returns true on
   * success.
   */
  boolean setEnabledAt(int index, boolean enabled);

  /**
   * Returns true if the specified |command_id| is checked. Only applies to
   * check and radio items.
   */
  boolean isChecked(int command_id);

  /**
   * Returns true if the specified |index| is checked. Only applies to check
   * and radio items.
   */
  boolean isCheckedAt(int index);

  /**
   * Check the specified |command_id|. Only applies to check and radio items.
   * Returns true on success.
   */
  boolean setChecked(int command_id, boolean checked);

  /**
   * Check the specified |index|. Only applies to check and radio items. Returns
   * true on success.
   */
  boolean setCheckedAt(int index, boolean checked);

  /**
   * Returns true if the specified |command_id| has a keyboard accelerator
   * assigned.
   */
  boolean hasAccelerator(int command_id);

  /**
   * Returns true if the specified |index| has a keyboard accelerator assigned.
   */
  boolean hasAcceleratorAt(int index);

  /**
   * Set the keyboard accelerator for the specified |command_id|. |key_code| can
   * be any key or character value. Returns true on success.
   */
  boolean setAccelerator(int command_id,
                         int key_code,
                         boolean shift_pressed,
                         boolean ctrl_pressed,
                         boolean alt_pressed);

  /**
   * Set the keyboard accelerator at the specified |index|. |key_code| can be
   * any key or character value. Returns true on success.
   */
  boolean setAcceleratorAt(int index,
                           int key_code,
                           boolean shift_pressed,
                           boolean ctrl_pressed,
                           boolean alt_pressed);

  /**
   * Remove the keyboard accelerator for the specified |command_id|. Returns
   * true on success.
   */
  boolean removeAccelerator(int command_id);

  /**
   * Remove the keyboard accelerator at the specified |index|. Returns true on
   * success.
   */
  boolean removeAcceleratorAt(int index);

  /**
   * Retrieves the keyboard accelerator for the specified |command_id|. Returns
   * true on success.
   */
  boolean getAccelerator(int command_id,
                         IntRef key_code,
                         BoolRef shift_pressed,
                         BoolRef ctrl_pressed,
                         BoolRef alt_pressed);

  /**
   * Retrieves the keyboard accelerator for the specified |index|. Returns true
   * on success.
   */
  boolean getAcceleratorAt(int index,
                           IntRef key_code,
                           BoolRef shift_pressed,
                           BoolRef ctrl_pressed,
                           BoolRef alt_pressed);
}
