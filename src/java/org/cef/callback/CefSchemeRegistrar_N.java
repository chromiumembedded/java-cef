// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.callback;

class CefSchemeRegistrar_N implements CefSchemeRegistrar {
  // Used internally to store a pointer to the CEF object.
  private long N_CefHandle = 0;

  @Override
  public void setNativeRef(String identifer, long nativeRef) {
    N_CefHandle = nativeRef;
  }

  @Override
  public long getNativeRef(String identifer) {
    return N_CefHandle;
  }

  @Override
  public boolean addCustomScheme(String schemeName,
                                 boolean isStandard,
                                 boolean isLocal,
                                 boolean isDisplayIsolated) {
    try {
      return N_AddCustomScheme(schemeName, isStandard, isLocal, isDisplayIsolated);
    } catch (UnsatisfiedLinkError err) {
      err.printStackTrace();
    }
    return false;
  }

  private final native boolean N_AddCustomScheme(String schemeName,
                                                 boolean isStandard,
                                                 boolean isLocal,
                                                 boolean isDisplayIsolated);
}
