// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.callback;

class CefSchemeRegistrar_N extends CefNativeAdapter implements CefSchemeRegistrar {

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
