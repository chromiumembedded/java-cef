// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.browser;

import org.cef.callback.CefNative;
import org.cef.handler.CefMessageRouterHandler;

class CefMessageRouter_N extends CefMessageRouter implements CefNative {
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

  private CefMessageRouter_N(CefMessageRouterConfig config) {
    super(config);
  }

  public static final CefMessageRouter createNative(CefMessageRouterConfig config) {
    // keep a reference to the request and client objects.
    CefMessageRouter_N result = new CefMessageRouter_N(config);
    try {
      result.N_Create(config);
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    if (result.N_CefHandle == 0)
      return null;
    return result;
  }

  @Override
  public void dispose() {
    try {
      N_Dispose();
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
  }

  @Override
  public boolean addHandler(CefMessageRouterHandler handler, boolean first) {
    try {
      return N_AddHandler(handler, first);
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
      return false;
    }
  }

  @Override
  public boolean removeHandler(CefMessageRouterHandler handler) {
    try {
      return N_RemoveHandler(handler);
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
      return false;
    }
  }

  @Override
  public void cancelPending(CefBrowser browser, CefMessageRouterHandler handler) {
    try {
      N_CancelPending(browser, handler);
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
  }

  @Override
  public int getPendingCount(CefBrowser browser, CefMessageRouterHandler handler) {
    try {
      return N_GetPendingCount(browser, handler);
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
      return 0;
    }
  }

  private final native void N_Create(CefMessageRouterConfig config);
  private final native void N_Dispose();
  private final native boolean N_AddHandler(CefMessageRouterHandler handler,
                                            boolean first);
  private final native boolean N_RemoveHandler(CefMessageRouterHandler handler);
  private final native void N_CancelPending(CefBrowser browser,
                                            CefMessageRouterHandler handler);
  private final native int N_GetPendingCount(CefBrowser browser,
                                             CefMessageRouterHandler handler);
}
