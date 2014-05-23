// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.network;

import org.cef.callback.CefNative;

class CefWebPluginInfo_N implements CefNative, CefWebPluginInfo {
  // Used internally to store a pointer to the CEF object.
  private long N_CefHandle = 0;

  CefWebPluginInfo_N() {
  }

  @Override
  public void setNativeRef(String identifer, long nativeRef) {
    N_CefHandle = nativeRef;
  }

  @Override
  public long getNativeRef(String identifer) {
    return N_CefHandle;
  }

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
