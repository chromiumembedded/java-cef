// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.network;

import org.cef.callback.CefCompletionCallback;
import org.cef.callback.CefCookieVisitor;
import org.cef.callback.CefNative;

import java.util.Vector;

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
        if (globalInstance != null && globalInstance.N_CefHandle != 0) {
            // The global instance is still valid.
            return globalInstance;
        }

        CefCookieManager_N result = null;
        try {
            result = CefCookieManager_N.N_GetGlobalManager();
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }

        globalInstance = result;
        return globalInstance;
    }

    @Override
    public void dispose() {
        try {
            N_Dispose(N_CefHandle);
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
    }

    @Override
    public boolean visitAllCookies(CefCookieVisitor visitor) {
        try {
            return N_VisitAllCookies(N_CefHandle, visitor);
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
        return false;
    }

    @Override
    public boolean visitUrlCookies(String url, boolean includeHttpOnly, CefCookieVisitor visitor) {
        try {
            return N_VisitUrlCookies(N_CefHandle, url, includeHttpOnly, visitor);
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
        return false;
    }

    @Override
    public boolean setCookie(String url, CefCookie cookie) {
        try {
            return N_SetCookie(N_CefHandle, url, cookie);
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
        return false;
    }

    @Override
    public boolean deleteCookies(String url, String cookieName) {
        try {
            return N_DeleteCookies(N_CefHandle, url, cookieName);
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
        return false;
    }

    @Override
    public boolean flushStore(CefCompletionCallback handler) {
        try {
            return N_FlushStore(N_CefHandle, handler);
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
        return false;
    }

    private final static native CefCookieManager_N N_GetGlobalManager();
    private final native void N_Dispose(long self);
    private final native boolean N_VisitAllCookies(long self, CefCookieVisitor visitor);
    private final native boolean N_VisitUrlCookies(
            long self, String url, boolean includeHttpOnly, CefCookieVisitor visitor);
    private final native boolean N_SetCookie(long self, String url, CefCookie cookie);
    private final native boolean N_DeleteCookies(long self, String url, String cookieName);
    private final native boolean N_FlushStore(long self, CefCompletionCallback handler);
}
