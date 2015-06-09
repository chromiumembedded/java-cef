// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.callback;

import java.util.Map;
import java.util.Vector;

class CefCommandLine_N extends CefNativeAdapter implements CefCommandLine {

  @Override
  public void reset() {
    try {
      N_reset();
    } catch (UnsatisfiedLinkError err) {
      err.printStackTrace();
    }
  }

  @Override
  public String getProgram() {
    try {
      return N_getProgram();
    } catch (UnsatisfiedLinkError err) {
      err.printStackTrace();
    }
    return null;
  }

  @Override
  public void setProgram(String program) {
    try {
      N_setProgram(program);
    } catch (UnsatisfiedLinkError err) {
      err.printStackTrace();
    }
  }

  @Override
  public boolean hasSwitches() {
    try {
      return N_hasSwitches();
    } catch (UnsatisfiedLinkError err) {
      err.printStackTrace();
    }
    return false;
  }

  @Override
  public boolean hasSwitch(String name) {
    try {
      return N_hasSwitch(name);
    } catch (UnsatisfiedLinkError err) {
      err.printStackTrace();
    }
    return false;
  }

  @Override
  public String getSwitchValue(String name) {
    try {
      return N_getSwitchValue(name);
    } catch (UnsatisfiedLinkError err) {
      err.printStackTrace();
    }
    return null;
  }

  @Override
  public Map<String, String> getSwitches() {
    try {
      return N_getSwitches();
    } catch (UnsatisfiedLinkError err) {
      err.printStackTrace();
    }
    return null;
  }

  @Override
  public void appendSwitch(String name) {
    try {
      N_appendSwitch(name);
    } catch (UnsatisfiedLinkError err) {
      err.printStackTrace();
    }
  }

  @Override
  public void appendSwitchWithValue(String name, String value) {
    try {
      N_appendSwitchWithValue(name, value);
    } catch (UnsatisfiedLinkError err) {
      err.printStackTrace();
    }
  }

  @Override
  public boolean hasArguments() {
    try {
      return N_hasArguments();
    } catch (UnsatisfiedLinkError err) {
      err.printStackTrace();
    }
    return false;
  }

  @Override
  public Vector<String> getArguments() {
    try {
      return N_getArguments();
    } catch (UnsatisfiedLinkError err) {
      err.printStackTrace();
    }
    return null;
  }

  @Override
  public void appendArgument(String argument) {
    try {
      N_appendArgument(argument);
    } catch (UnsatisfiedLinkError err) {
      err.printStackTrace();
    }
  }

  @Override
  public String toString() {
    String result = "CefCommandLine [program=\'" + getProgram() + "\'";
    if (hasSwitches()) {
      Map<String,String> switches = getSwitches();
      result += ", switches=" + switches;
    }
    if (hasArguments()) {
      Vector<String> arguments = getArguments();
      result += ", arguments=" + arguments;
    }
    return result + "]";
  }

  private final native void N_reset();
  private final native String N_getProgram();
  private final native void N_setProgram(String program);
  private final native boolean N_hasSwitches();
  private final native boolean N_hasSwitch(String name);
  private final native String N_getSwitchValue(String name);
  private final native Map<String, String> N_getSwitches();
  private final native void N_appendSwitch(String name);
  private final native void N_appendSwitchWithValue(String name, String value);
  private final native boolean N_hasArguments();
  private final native Vector<String> N_getArguments();
  private final native void N_appendArgument(String argument);
}
