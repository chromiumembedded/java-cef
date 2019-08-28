// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.network;

import org.cef.callback.CefNative;

import java.util.Vector;

/**
 *
 */
class CefPostData_N extends CefPostData implements CefNative {
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

    CefPostData_N() {
        super();
    }

    public static CefPostData createNative() {
        try {
            return CefPostData_N.N_Create();
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
            return null;
        }
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
    public boolean isReadOnly() {
        try {
            return N_IsReadOnly(N_CefHandle);
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
        return false;
    }

    @Override
    public int getElementCount() {
        try {
            return N_GetElementCount(N_CefHandle);
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
        return 0;
    }

    @Override
    public void getElements(Vector<CefPostDataElement> elements) {
        try {
            N_GetElements(N_CefHandle, elements);
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
    }

    @Override
    public boolean removeElement(CefPostDataElement element) {
        try {
            return N_RemoveElement(N_CefHandle, element);
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
        return false;
    }

    @Override
    public boolean addElement(CefPostDataElement element) {
        try {
            return N_AddElement(N_CefHandle, element);
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
        return false;
    }

    @Override
    public void removeElements() {
        try {
            N_RemoveElements(N_CefHandle);
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
    }

    private final native static CefPostData_N N_Create();
    private final native void N_Dispose(long self);
    private final native boolean N_IsReadOnly(long self);
    private final native int N_GetElementCount(long self);
    private final native void N_GetElements(long self, Vector<CefPostDataElement> elements);
    private final native boolean N_RemoveElement(long self, CefPostDataElement element);
    private final native boolean N_AddElement(long self, CefPostDataElement element);
    private final native void N_RemoveElements(long self);
}
