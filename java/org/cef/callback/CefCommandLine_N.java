// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.callback;

import java.util.Map;
import java.util.Vector;

class CefCommandLine_N extends CefNativeAdapter implements CefCommandLine {
    CefCommandLine_N() {}

    @Override
    public void reset() {
        try {
            N_Reset(getNativeRef(null));
        } catch (UnsatisfiedLinkError err) {
            err.printStackTrace();
        }
    }

    @Override
    public String getProgram() {
        try {
            return N_GetProgram(getNativeRef(null));
        } catch (UnsatisfiedLinkError err) {
            err.printStackTrace();
        }
        return null;
    }

    @Override
    public void setProgram(String program) {
        try {
            N_SetProgram(getNativeRef(null), program);
        } catch (UnsatisfiedLinkError err) {
            err.printStackTrace();
        }
    }

    @Override
    public boolean hasSwitches() {
        try {
            return N_HasSwitches(getNativeRef(null));
        } catch (UnsatisfiedLinkError err) {
            err.printStackTrace();
        }
        return false;
    }

    @Override
    public boolean hasSwitch(String name) {
        try {
            return N_HasSwitch(getNativeRef(null), name);
        } catch (UnsatisfiedLinkError err) {
            err.printStackTrace();
        }
        return false;
    }

    @Override
    public String getSwitchValue(String name) {
        try {
            return N_GetSwitchValue(getNativeRef(null), name);
        } catch (UnsatisfiedLinkError err) {
            err.printStackTrace();
        }
        return null;
    }

    @Override
    public Map<String, String> getSwitches() {
        try {
            return N_GetSwitches(getNativeRef(null));
        } catch (UnsatisfiedLinkError err) {
            err.printStackTrace();
        }
        return null;
    }

    @Override
    public void appendSwitch(String name) {
        try {
            N_AppendSwitch(getNativeRef(null), name);
        } catch (UnsatisfiedLinkError err) {
            err.printStackTrace();
        }
    }

    @Override
    public void appendSwitchWithValue(String name, String value) {
        try {
            N_AppendSwitchWithValue(getNativeRef(null), name, value);
        } catch (UnsatisfiedLinkError err) {
            err.printStackTrace();
        }
    }

    @Override
    public boolean hasArguments() {
        try {
            return N_HasArguments(getNativeRef(null));
        } catch (UnsatisfiedLinkError err) {
            err.printStackTrace();
        }
        return false;
    }

    @Override
    public Vector<String> getArguments() {
        try {
            return N_GetArguments(getNativeRef(null));
        } catch (UnsatisfiedLinkError err) {
            err.printStackTrace();
        }
        return null;
    }

    @Override
    public void appendArgument(String argument) {
        try {
            N_AppendArgument(getNativeRef(null), argument);
        } catch (UnsatisfiedLinkError err) {
            err.printStackTrace();
        }
    }

    @Override
    public String toString() {
        String result = "CefCommandLine [program=\'" + getProgram() + "\'";
        if (hasSwitches()) {
            Map<String, String> switches = getSwitches();
            result += ", switches=" + switches;
        }
        if (hasArguments()) {
            Vector<String> arguments = getArguments();
            result += ", arguments=" + arguments;
        }
        return result + "]";
    }

    private final native void N_Reset(long self);
    private final native String N_GetProgram(long self);
    private final native void N_SetProgram(long self, String program);
    private final native boolean N_HasSwitches(long self);
    private final native boolean N_HasSwitch(long self, String name);
    private final native String N_GetSwitchValue(long self, String name);
    private final native Map<String, String> N_GetSwitches(long self);
    private final native void N_AppendSwitch(long self, String name);
    private final native void N_AppendSwitchWithValue(long self, String name, String value);
    private final native boolean N_HasArguments(long self);
    private final native Vector<String> N_GetArguments(long self);
    private final native void N_AppendArgument(long self, String argument);
}
