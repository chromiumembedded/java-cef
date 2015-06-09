// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.network;

import org.cef.callback.CefNativeAdapter;

class CefWebPluginInfo_N extends CefNativeAdapter implements CefWebPluginInfo {

  @Override
  public String getName() {
    try {
      return N_GetName();
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return null;
  }

  @Override
  public String getPath() {
    try {
      return N_GetPath();
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return null;
  }

  @Override
  public String getVersion() {
    try {
      return N_GetVersion();
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return null;
  }

  @Override
  public String getDescription() {
    try {
      return N_GetDescription();
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return null;
  }

  private final native String N_GetName();
  private final native String N_GetPath();
  private final native String N_GetVersion();
  private final native String N_GetDescription();
}
