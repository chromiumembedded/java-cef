// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.network;

import java.util.Vector;

import org.cef.callback.CefCompletionCallback;
import org.cef.callback.CefCookieVisitor;
import org.cef.callback.CefNative;

class CefCookieManager_N extends CefCookieManager implements CefNative {
  // Used internally to store a pointer to the CEF object.
  private long N_CefHandle = 0;
  private static CefCookieManager_N globalInstance = null;

  @Override
  public void setNativeRef(String identifer, long nativeRef) {
    N_CefHandle = nativeRef;
  }

  @Override
  public long getNativeRef(String identifer) {
    return N_CefHandle;
  }

  CefCookieManager_N() {
    super();
  }

  static synchronized final CefCookieManager_N getGlobalManagerNative() {
    CefCookieManager_N result = null;
    try {
      result = CefCookieManager_N.N_GetGlobalManager();
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }

    if (globalInstance == null) {
      globalInstance = result;
    } else if (globalInstance.N_CefHandle == result.N_CefHandle) {
      result.N_CefCookieManager_DTOR();
    }
    return globalInstance;
  }

  static final CefCookieManager_N createNative(String path,
                                               boolean persistSessionCookies) {
    CefCookieManager_N result = null;
    try {
      result = CefCookieManager_N.N_CreateManager(path, persistSessionCookies);
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return result;
  }

  @Override
  protected void finalize() throws Throwable {
    try {
      N_CefCookieManager_DTOR();
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    } finally {
      super.finalize();
    }
  }

  @Override
  public void setSupportedSchemes(Vector<String> schemes) {
    try {
      N_SetSupportedSchemes(schemes);
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
  }

  @Override
  public boolean visitAllCookies(CefCookieVisitor visitor) {
    try {
      return N_VisitAllCookies(visitor);
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return false;
  }

  @Override
  public boolean visitUrlCookies(String url,
                                 boolean includeHttpOnly,
                                 CefCookieVisitor visitor) {
    try {
      return N_VisitUrlCookies(url, includeHttpOnly, visitor);
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return false;
  }

  @Override
  public boolean setCookie(String url, CefCookie cookie) {
    try {
      return N_SetCookie(url, cookie);
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return false;
  }

  @Override
  public boolean deleteCookies(String url, String cookieName) {
    try {
      return N_DeleteCookies(url, cookieName);
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return false;
  }

  @Override
  public boolean setStoragePath(String path, boolean persistSessionCookies) {
    try {
      return N_SetStoragePath(path, persistSessionCookies);
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return false;
  }

  @Override
  public boolean flushStore(CefCompletionCallback handler) {
    try {
      return N_FlushStore(handler);
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return false;
  }

  private final static native CefCookieManager_N N_GetGlobalManager();
  private final static native CefCookieManager_N N_CreateManager(String path,
                                                                 boolean persistSessionCookies);
  private final native void N_SetSupportedSchemes(Vector<String> schemes);
  private final native boolean N_VisitAllCookies(CefCookieVisitor visitor);
  private final native boolean N_VisitUrlCookies(String url,
                                                 boolean includeHttpOnly,
                                                 CefCookieVisitor visitor);
  private final native boolean N_SetCookie(String url, CefCookie cookie);
  private final native boolean N_DeleteCookies(String url, String cookieName);
  private final native boolean N_SetStoragePath(String path,
                                                boolean persistSessionCookies);
  private final native boolean N_FlushStore(CefCompletionCallback handler);
  private final native void N_CefCookieManager_DTOR();
}
