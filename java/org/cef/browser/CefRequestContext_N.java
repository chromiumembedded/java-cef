// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.browser;

import org.cef.callback.CefNative;
import org.cef.handler.CefRequestContextHandler;

class CefRequestContext_N extends CefRequestContext implements CefNative {
  // Used internally to store a pointer to the CEF object.
  private long N_CefHandle = 0;
  private static CefRequestContext_N globalInstance = null;
  private CefRequestContextHandler handler = null;

  @Override
  public void setNativeRef(String identifer, long nativeRef) {
    N_CefHandle = nativeRef;
  }

  @Override
  public long getNativeRef(String identifer) {
    return N_CefHandle;
  }

  CefRequestContext_N() {
    super();
  }

  static final CefRequestContext_N getGlobalContextNative() {
    CefRequestContext_N result = null;
    try {
      result = CefRequestContext_N.N_GetGlobalContext();
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }

    if (globalInstance == null) {
      globalInstance = result;
    } else if (globalInstance.N_CefHandle == result.N_CefHandle) {
      result.N_CefRequestContext_DTOR();
    }
    return globalInstance;
  }

  static final CefRequestContext_N createNative(CefRequestContextHandler handler) {
    CefRequestContext_N result = null;
    try {
      result = CefRequestContext_N.N_CreateContext(handler);
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    if (result != null)
      result.handler = handler;
    return result;
  }

  @Override
  public void dispose() {
    try {
      N_CefRequestContext_DTOR();
    } catch(UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
  }

  @Override
  public boolean isGlobal() {
    try {
      return N_IsGlobal();
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return false;
  }

  @Override
  public CefRequestContextHandler getHandler() {
    return handler;
  }

  private final static native CefRequestContext_N N_GetGlobalContext();
  private final static native CefRequestContext_N N_CreateContext(CefRequestContextHandler handler);
  private final native boolean N_IsGlobal();
  private final native void N_CefRequestContext_DTOR();
}
