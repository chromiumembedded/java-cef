// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.callback;

import org.cef.misc.BoolRef;
import org.cef.misc.IntRef;

class CefMenuModel_N extends CefNativeAdapter implements CefMenuModel {
    public CefMenuModel_N() {}

    @Override
    public boolean clear() {
        try {
            return N_Clear(getNativeRef(null));
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
        return false;
    }

    @Override
    public int getCount() {
        try {
            return N_GetCount(getNativeRef(null));
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
        return 0;
    }

    @Override
    public boolean addSeparator() {
        try {
            return N_AddSeparator(getNativeRef(null));
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
        return false;
    }

    @Override
    public boolean addItem(int command_id, String label) {
        try {
            return N_AddItem(getNativeRef(null), command_id, label);
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
        return false;
    }

    @Override
    public boolean addCheckItem(int command_id, String label) {
        try {
            return N_AddCheckItem(getNativeRef(null), command_id, label);
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
        return false;
    }

    @Override
    public boolean addRadioItem(int command_id, String label, int group_id) {
        try {
            return N_AddRadioItem(getNativeRef(null), command_id, label, group_id);
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
        return false;
    }

    @Override
    public CefMenuModel addSubMenu(int command_id, String label) {
        try {
            return N_AddSubMenu(getNativeRef(null), command_id, label);
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
        return null;
    }

    @Override
    public boolean insertSeparatorAt(int index) {
        try {
            return N_InsertSeparatorAt(getNativeRef(null), index);
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
        return false;
    }

    @Override
    public boolean insertItemAt(int index, int command_id, String label) {
        try {
            return N_InsertItemAt(getNativeRef(null), index, command_id, label);
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
        return false;
    }

    @Override
    public boolean insertCheckItemAt(int index, int command_id, String label) {
        try {
            return N_InsertCheckItemAt(getNativeRef(null), index, command_id, label);
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
        return false;
    }

    @Override
    public boolean insertRadioItemAt(int index, int command_id, String label, int group_id) {
        try {
            return N_InsertRadioItemAt(getNativeRef(null), index, command_id, label, group_id);
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
        return false;
    }

    @Override
    public CefMenuModel insertSubMenuAt(int index, int command_id, String label) {
        try {
            return N_InsertSubMenuAt(getNativeRef(null), index, command_id, label);
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
        return null;
    }

    @Override
    public boolean remove(int command_id) {
        try {
            return N_Remove(getNativeRef(null), command_id);
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
        return false;
    }

    @Override
    public boolean removeAt(int index) {
        try {
            return N_RemoveAt(getNativeRef(null), index);
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
        return false;
    }

    @Override
    public int getIndexOf(int command_id) {
        try {
            return N_GetIndexOf(getNativeRef(null), command_id);
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
        return 0;
    }

    @Override
    public int getCommandIdAt(int index) {
        try {
            return N_GetCommandIdAt(getNativeRef(null), index);
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
        return 0;
    }

    @Override
    public boolean setCommandIdAt(int index, int command_id) {
        try {
            return N_SetCommandIdAt(getNativeRef(null), index, command_id);
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
        return false;
    }

    @Override
    public String getLabel(int command_id) {
        try {
            return N_GetLabel(getNativeRef(null), command_id);
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
        return null;
    }

    @Override
    public String getLabelAt(int index) {
        try {
            return N_GetLabelAt(getNativeRef(null), index);
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
        return null;
    }

    @Override
    public boolean setLabel(int command_id, String label) {
        try {
            return N_SetLabel(getNativeRef(null), command_id, label);
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
        return false;
    }

    @Override
    public boolean setLabelAt(int index, String label) {
        try {
            return N_SetLabelAt(getNativeRef(null), index, label);
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
        return false;
    }

    @Override
    public MenuItemType getType(int command_id) {
        try {
            return N_GetType(getNativeRef(null), command_id);
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
        return null;
    }

    @Override
    public MenuItemType getTypeAt(int index) {
        try {
            return N_GetTypeAt(getNativeRef(null), index);
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
        return null;
    }

    @Override
    public int getGroupId(int command_id) {
        try {
            return N_GetGroupId(getNativeRef(null), command_id);
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
        return 0;
    }

    @Override
    public int getGroupIdAt(int index) {
        try {
            return N_GetGroupIdAt(getNativeRef(null), index);
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
        return 0;
    }

    @Override
    public boolean setGroupId(int command_id, int group_id) {
        try {
            return N_SetGroupId(getNativeRef(null), command_id, group_id);
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
        return false;
    }

    @Override
    public boolean setGroupIdAt(int index, int group_id) {
        try {
            return N_SetGroupIdAt(getNativeRef(null), index, group_id);
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
        return false;
    }

    @Override
    public CefMenuModel getSubMenu(int command_id) {
        try {
            return N_GetSubMenu(getNativeRef(null), command_id);
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
        return null;
    }

    @Override
    public CefMenuModel getSubMenuAt(int index) {
        try {
            return N_GetSubMenuAt(getNativeRef(null), index);
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
        return null;
    }

    @Override
    public boolean isVisible(int command_id) {
        try {
            return N_IsVisible(getNativeRef(null), command_id);
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
        return false;
    }

    @Override
    public boolean isVisibleAt(int index) {
        try {
            return N_IsVisibleAt(getNativeRef(null), index);
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
        return false;
    }

    @Override
    public boolean setVisible(int command_id, boolean visible) {
        try {
            return N_SetVisible(getNativeRef(null), command_id, visible);
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
        return false;
    }

    @Override
    public boolean setVisibleAt(int index, boolean visible) {
        try {
            return N_SetVisibleAt(getNativeRef(null), index, visible);
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
        return false;
    }

    @Override
    public boolean isEnabled(int command_id) {
        try {
            return N_IsEnabled(getNativeRef(null), command_id);
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
        return false;
    }

    @Override
    public boolean isEnabledAt(int index) {
        try {
            return N_IsEnabledAt(getNativeRef(null), index);
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
        return false;
    }

    @Override
    public boolean setEnabled(int command_id, boolean enabled) {
        try {
            return N_SetEnabled(getNativeRef(null), command_id, enabled);
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
        return false;
    }

    @Override
    public boolean setEnabledAt(int index, boolean enabled) {
        try {
            return N_SetEnabledAt(getNativeRef(null), index, enabled);
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
        return false;
    }

    @Override
    public boolean isChecked(int command_id) {
        try {
            return N_IsChecked(getNativeRef(null), command_id);
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
        return false;
    }

    @Override
    public boolean isCheckedAt(int index) {
        try {
            return N_IsCheckedAt(getNativeRef(null), index);
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
        return false;
    }

    @Override
    public boolean setChecked(int command_id, boolean checked) {
        try {
            return N_SetChecked(getNativeRef(null), command_id, checked);
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
        return false;
    }

    @Override
    public boolean setCheckedAt(int index, boolean checked) {
        try {
            return N_SetCheckedAt(getNativeRef(null), index, checked);
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
        return false;
    }

    @Override
    public boolean hasAccelerator(int command_id) {
        try {
            return N_HasAccelerator(getNativeRef(null), command_id);
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
        return false;
    }

    @Override
    public boolean hasAcceleratorAt(int index) {
        try {
            return N_HasAcceleratorAt(getNativeRef(null), index);
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
        return false;
    }

    @Override
    public boolean setAccelerator(int command_id, int key_code, boolean shift_pressed,
            boolean ctrl_pressed, boolean alt_pressed) {
        try {
            return N_SetAccelerator(getNativeRef(null), command_id, key_code, shift_pressed,
                    ctrl_pressed, alt_pressed);
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
        return false;
    }

    @Override
    public boolean setAcceleratorAt(int index, int key_code, boolean shift_pressed,
            boolean ctrl_pressed, boolean alt_pressed) {
        try {
            return N_SetAcceleratorAt(
                    getNativeRef(null), index, key_code, shift_pressed, ctrl_pressed, alt_pressed);
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
        return false;
    }

    @Override
    public boolean removeAccelerator(int command_id) {
        try {
            return N_RemoveAccelerator(getNativeRef(null), command_id);
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
        return false;
    }

    @Override
    public boolean removeAcceleratorAt(int index) {
        try {
            return N_RemoveAcceleratorAt(getNativeRef(null), index);
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
        return false;
    }

    @Override
    public boolean getAccelerator(int command_id, IntRef key_code, BoolRef shift_pressed,
            BoolRef ctrl_pressed, BoolRef alt_pressed) {
        try {
            return N_GetAccelerator(getNativeRef(null), command_id, key_code, shift_pressed,
                    ctrl_pressed, alt_pressed);
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
        return false;
    }

    @Override
    public boolean getAcceleratorAt(int index, IntRef key_code, BoolRef shift_pressed,
            BoolRef ctrl_pressed, BoolRef alt_pressed) {
        try {
            return N_GetAcceleratorAt(
                    getNativeRef(null), index, key_code, shift_pressed, ctrl_pressed, alt_pressed);
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
        return false;
    }

    private final native boolean N_Clear(long self);
    private final native int N_GetCount(long self);
    private final native boolean N_AddSeparator(long self);
    private final native boolean N_AddItem(long self, int command_id, String label);
    private final native boolean N_AddCheckItem(long self, int command_id, String label);
    private final native boolean N_AddRadioItem(
            long self, int command_id, String label, int group_id);
    private final native CefMenuModel N_AddSubMenu(long self, int command_id, String label);
    private final native boolean N_InsertSeparatorAt(long self, int index);
    private final native boolean N_InsertItemAt(long self, int index, int command_id, String label);
    private final native boolean N_InsertCheckItemAt(
            long self, int index, int command_id, String label);
    private final native boolean N_InsertRadioItemAt(
            long self, int index, int command_id, String label, int group_id);
    private final native CefMenuModel N_InsertSubMenuAt(
            long self, int index, int command_id, String label);
    private final native boolean N_Remove(long self, int command_id);
    private final native boolean N_RemoveAt(long self, int index);
    private final native int N_GetIndexOf(long self, int command_id);
    private final native int N_GetCommandIdAt(long self, int index);
    private final native boolean N_SetCommandIdAt(long self, int index, int command_id);
    private final native String N_GetLabel(long self, int command_id);
    private final native String N_GetLabelAt(long self, int index);
    private final native boolean N_SetLabel(long self, int command_id, String label);
    private final native boolean N_SetLabelAt(long self, int index, String label);
    private final native MenuItemType N_GetType(long self, int command_id);
    private final native MenuItemType N_GetTypeAt(long self, int index);
    private final native int N_GetGroupId(long self, int command_id);
    private final native int N_GetGroupIdAt(long self, int index);
    private final native boolean N_SetGroupId(long self, int command_id, int group_id);
    private final native boolean N_SetGroupIdAt(long self, int index, int group_id);
    private final native CefMenuModel N_GetSubMenu(long self, int command_id);
    private final native CefMenuModel N_GetSubMenuAt(long self, int index);
    private final native boolean N_IsVisible(long self, int command_id);
    private final native boolean N_IsVisibleAt(long self, int index);
    private final native boolean N_SetVisible(long self, int command_id, boolean visible);
    private final native boolean N_SetVisibleAt(long self, int index, boolean visible);
    private final native boolean N_IsEnabled(long self, int command_id);
    private final native boolean N_IsEnabledAt(long self, int index);
    private final native boolean N_SetEnabled(long self, int command_id, boolean enabled);
    private final native boolean N_SetEnabledAt(long self, int index, boolean enabled);
    private final native boolean N_IsChecked(long self, int command_id);
    private final native boolean N_IsCheckedAt(long self, int index);
    private final native boolean N_SetChecked(long self, int command_id, boolean checked);
    private final native boolean N_SetCheckedAt(long self, int index, boolean checked);
    private final native boolean N_HasAccelerator(long self, int command_id);
    private final native boolean N_HasAcceleratorAt(long self, int index);
    private final native boolean N_SetAccelerator(long self, int command_id, int key_code,
            boolean shift_pressed, boolean ctrl_pressed, boolean alt_pressed);
    private final native boolean N_SetAcceleratorAt(long self, int index, int key_code,
            boolean shift_pressed, boolean ctrl_pressed, boolean alt_pressed);
    private final native boolean N_RemoveAccelerator(long self, int command_id);
    private final native boolean N_RemoveAcceleratorAt(long self, int index);
    private final native boolean N_GetAccelerator(long self, int command_id, IntRef key_code,
            BoolRef shift_pressed, BoolRef ctrl_pressed, BoolRef alt_pressed);
    private final native boolean N_GetAcceleratorAt(long self, int index, IntRef key_code,
            BoolRef shift_pressed, BoolRef ctrl_pressed, BoolRef alt_pressed);
}
