// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.callback;

import org.cef.misc.BoolRef;
import org.cef.misc.IntRef;

class CefMenuModel_N extends CefNativeAdapter implements CefMenuModel {

  public CefMenuModel_N() {
  }

  @Override
  public boolean clear() {
    try {
      return N_Clear();
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return false;
  }

  @Override
  public int getCount() {
    try {
      return N_GetCount();
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return 0;
  }

  @Override
  public boolean addSeparator() {
    try {
      return N_AddSeparator();
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return false;
  }

  @Override
  public boolean addItem(int command_id, String label) {
    try {
      return N_AddItem(command_id, label);
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return false;
  }

  @Override
  public boolean addCheckItem(int command_id, String label) {
    try {
      return N_AddCheckItem(command_id, label);
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return false;
  }

  @Override
  public boolean addRadioItem(int command_id, String label, int group_id) {
    try {
      return N_AddRadioItem(command_id, label, group_id);
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return false;
  }

  @Override
  public CefMenuModel addSubMenu(int command_id, String label) {
    try {
      return N_AddSubMenu(command_id, label);
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return null;
  }

  @Override
  public boolean insertSeparatorAt(int index) {
    try {
      return N_InsertSeparatorAt(index);
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return false;
  }

  @Override
  public boolean insertItemAt(int index, int command_id, String label) {
    try {
      return N_InsertItemAt(index, command_id, label);
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return false;
  }

  @Override
  public boolean insertCheckItemAt(int index, int command_id, String label) {
    try {
      return N_InsertCheckItemAt(index, command_id, label);
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return false;
  }

  @Override
  public boolean insertRadioItemAt(int index, int command_id,
                                   String label, int group_id) {
    try {
      return N_InsertRadioItemAt(index, command_id, label, group_id);
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return false;
  }

  @Override
  public CefMenuModel insertSubMenuAt(int index, int command_id, String label) {
    try {
      return N_InsertSubMenuAt(index, command_id, label);
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return null;
  }

  @Override
  public boolean remove(int command_id) {
    try {
      return N_Remove(command_id);
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return false;
  }

  @Override
  public boolean removeAt(int index) {
    try {
      return N_RemoveAt(index);
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return false;
  }

  @Override
  public int getIndexOf(int command_id) {
    try {
      return N_GetIndexOf(command_id);
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return 0;
  }

  @Override
  public int getCommandIdAt(int index) {
    try {
      return N_GetCommandIdAt(index);
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return 0;
  }

  @Override
  public boolean setCommandIdAt(int index, int command_id) {
    try {
      return N_SetCommandIdAt(index, command_id);
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return false;
  }

  @Override
  public String getLabel(int command_id) {
    try {
      return N_GetLabel(command_id);
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return null;
  }

  @Override
  public String getLabelAt(int index) {
    try {
      return N_GetLabelAt(index);
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return null;
  }

  @Override
  public boolean setLabel(int command_id, String label) {
    try {
      return N_SetLabel(command_id, label);
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return false;
  }

  @Override
  public boolean setLabelAt(int index, String label) {
    try {
      return N_SetLabelAt(index, label);
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return false;
  }

  @Override
  public MenuItemType getType(int command_id) {
    try {
      return N_GetType(command_id);
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return null;
  }

  @Override
  public MenuItemType getTypeAt(int index) {
    try {
      return N_GetTypeAt(index);
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return null;
  }

  @Override
  public int getGroupId(int command_id) {
    try {
      return N_GetGroupId(command_id);
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return 0;
  }

  @Override
  public int getGroupIdAt(int index) {
    try {
      return N_GetGroupIdAt(index);
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return 0;
  }

  @Override
  public boolean setGroupId(int command_id, int group_id) {
    try {
      return N_SetGroupId(command_id, group_id);
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return false;
  }

  @Override
  public boolean setGroupIdAt(int index, int group_id) {
    try {
      return N_SetGroupIdAt(index, group_id);
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return false;
  }

  @Override
  public CefMenuModel getSubMenu(int command_id) {
    try {
      return N_GetSubMenu(command_id);
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return null;
  }

  @Override
  public CefMenuModel getSubMenuAt(int index) {
    try {
      return N_GetSubMenuAt(index);
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return null;
  }

  @Override
  public boolean isVisible(int command_id) {
    try {
      return N_IsVisible(command_id);
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return false;
  }

  @Override
  public boolean isVisibleAt(int index) {
    try {
      return N_IsVisibleAt(index);
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return false;
  }

  @Override
  public boolean setVisible(int command_id, boolean visible) {
    try {
      return N_SetVisible(command_id, visible);
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return false;
  }

  @Override
  public boolean setVisibleAt(int index, boolean visible) {
    try {
      return N_SetVisibleAt(index, visible);
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return false;
  }

  @Override
  public boolean isEnabled(int command_id) {
    try {
      return N_IsEnabled(command_id);
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return false;
  }

  @Override
  public boolean isEnabledAt(int index) {
    try {
      return N_IsEnabledAt(index);
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return false;
  }

  @Override
  public boolean setEnabled(int command_id, boolean enabled) {
    try {
      return N_SetEnabled(command_id, enabled);
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return false;
  }

  @Override
  public boolean setEnabledAt(int index, boolean enabled) {
    try {
      return N_SetEnabledAt(index, enabled);
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return false;
  }

  @Override
  public boolean isChecked(int command_id) {
    try {
      return N_IsChecked(command_id);
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return false;
  }

  @Override
  public boolean isCheckedAt(int index) {
    try {
      return N_IsCheckedAt(index);
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return false;
  }

  @Override
  public boolean setChecked(int command_id, boolean checked) {
    try {
      return N_SetChecked(command_id, checked);
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return false;
  }

  @Override
  public boolean setCheckedAt(int index, boolean checked) {
    try {
      return N_SetCheckedAt(index, checked);
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return false;
  }

  @Override
  public boolean hasAccelerator(int command_id) {
    try {
      return N_HasAccelerator(command_id);
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return false;
  }

  @Override
  public boolean hasAcceleratorAt(int index) {
    try {
      return N_HasAcceleratorAt(index);
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return false;
  }

  @Override
  public boolean setAccelerator(int command_id,
                                int key_code,
                                boolean shift_pressed,
                                boolean ctrl_pressed,
                                boolean alt_pressed) {
    try {
      return N_SetAccelerator(command_id, key_code, shift_pressed, ctrl_pressed, alt_pressed);
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return false;
  }

  @Override
  public boolean setAcceleratorAt(int index,
                                  int key_code,
                                  boolean shift_pressed,
                                  boolean ctrl_pressed,
                                  boolean alt_pressed) {
    try {
      return N_SetAcceleratorAt(index, key_code, shift_pressed, ctrl_pressed, alt_pressed);
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return false;
  }

  @Override
  public boolean removeAccelerator(int command_id) {
    try {
      return N_RemoveAccelerator(command_id);
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return false;
  }

  @Override
  public boolean removeAcceleratorAt(int index) {
    try {
      return N_RemoveAcceleratorAt(index);
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return false;
  }

  @Override
  public boolean getAccelerator(int command_id,
                                IntRef key_code,
                                BoolRef shift_pressed,
                                BoolRef ctrl_pressed,
                                BoolRef alt_pressed) {
    try {
      return N_GetAccelerator(command_id, key_code, shift_pressed, ctrl_pressed, alt_pressed);
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return false;
  }

  @Override
  public boolean getAcceleratorAt(int index,
                                  IntRef key_code,
                                  BoolRef shift_pressed,
                                  BoolRef ctrl_pressed,
                                  BoolRef alt_pressed) {
    try {
      return N_GetAcceleratorAt(index, key_code, shift_pressed, ctrl_pressed, alt_pressed);
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return false;
  }

  private final native boolean N_Clear();
  private final native int N_GetCount();
  private final native boolean N_AddSeparator();
  private final native boolean N_AddItem(int command_id, String label);
  private final native boolean N_AddCheckItem(int command_id, String label);
  private final native boolean N_AddRadioItem(int command_id, String label, int group_id);
  private final native CefMenuModel N_AddSubMenu(int command_id, String label);
  private final native boolean N_InsertSeparatorAt(int index);
  private final native boolean N_InsertItemAt(int index, int command_id, String label);
  private final native boolean N_InsertCheckItemAt(int index, int command_id, String label);
  private final native boolean N_InsertRadioItemAt(int index, int command_id, String label, int group_id);
  private final native CefMenuModel N_InsertSubMenuAt(int index, int command_id, String label);
  private final native boolean N_Remove(int command_id);
  private final native boolean N_RemoveAt(int index);
  private final native int N_GetIndexOf(int command_id);
  private final native int N_GetCommandIdAt(int index);
  private final native boolean N_SetCommandIdAt(int index, int command_id);
  private final native String N_GetLabel(int command_id);
  private final native String N_GetLabelAt(int index);
  private final native boolean N_SetLabel(int command_id, String label);
  private final native boolean N_SetLabelAt(int index, String label);
  private final native MenuItemType N_GetType(int command_id);
  private final native MenuItemType N_GetTypeAt(int index);
  private final native int N_GetGroupId(int command_id);
  private final native int N_GetGroupIdAt(int index);
  private final native boolean N_SetGroupId(int command_id, int group_id);
  private final native boolean N_SetGroupIdAt(int index, int group_id);
  private final native CefMenuModel N_GetSubMenu(int command_id);
  private final native CefMenuModel N_GetSubMenuAt(int index);
  private final native boolean N_IsVisible(int command_id);
  private final native boolean N_IsVisibleAt(int index);
  private final native boolean N_SetVisible(int command_id, boolean visible);
  private final native boolean N_SetVisibleAt(int index, boolean visible);
  private final native boolean N_IsEnabled(int command_id);
  private final native boolean N_IsEnabledAt(int index);
  private final native boolean N_SetEnabled(int command_id, boolean enabled);
  private final native boolean N_SetEnabledAt(int index, boolean enabled);
  private final native boolean N_IsChecked(int command_id);
  private final native boolean N_IsCheckedAt(int index);
  private final native boolean N_SetChecked(int command_id, boolean checked);
  private final native boolean N_SetCheckedAt(int index, boolean checked);
  private final native boolean N_HasAccelerator(int command_id);
  private final native boolean N_HasAcceleratorAt(int index);
  private final native boolean N_SetAccelerator(int command_id,
                                                int key_code,
                                                boolean shift_pressed,
                                                boolean ctrl_pressed,
                                                boolean alt_pressed);
  private final native boolean N_SetAcceleratorAt(int index,
                                                  int key_code,
                                                  boolean shift_pressed,
                                                  boolean ctrl_pressed,
                                                  boolean alt_pressed);
  private final native boolean N_RemoveAccelerator(int command_id);
  private final native boolean N_RemoveAcceleratorAt(int index);
  private final native boolean N_GetAccelerator(int command_id,
                                                IntRef key_code,
                                                BoolRef shift_pressed,
                                                BoolRef ctrl_pressed,
                                                BoolRef alt_pressed);
  private final native boolean N_GetAcceleratorAt(int index,
                                                  IntRef key_code,
                                                  BoolRef shift_pressed,
                                                  BoolRef ctrl_pressed,
                                                  BoolRef alt_pressed);
}
