// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.network;

import org.cef.callback.CefNative;

class CefPostDataElement_N extends CefPostDataElement implements CefNative {
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

    CefPostDataElement_N() {
        super();
    }

    public static CefPostDataElement createNative() {
        try {
            return CefPostDataElement_N.N_Create();
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
    public void setToEmpty() {
        try {
            N_SetToEmpty(N_CefHandle);
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
    }

    @Override
    public void setToFile(String fileName) {
        try {
            N_SetToFile(N_CefHandle, fileName);
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
    }

    @Override
    public void setToBytes(int size, byte[] bytes) {
        try {
            N_SetToBytes(N_CefHandle, size, bytes);
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
    }

    @Override
    public Type getType() {
        try {
            return N_GetType(N_CefHandle);
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
        return null;
    }

    @Override
    public String getFile() {
        try {
            return N_GetFile(N_CefHandle);
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
        return null;
    }

    @Override
    public int getBytesCount() {
        try {
            return N_GetBytesCount(N_CefHandle);
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
        return 0;
    }

    @Override
    public int getBytes(int size, byte[] bytes) {
        try {
            return N_GetBytes(N_CefHandle, size, bytes);
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
        return 0;
    }

    private final native static CefPostDataElement_N N_Create();
    private final native void N_Dispose(long self);
    private final native boolean N_IsReadOnly(long self);
    private final native void N_SetToEmpty(long self);
    private final native void N_SetToFile(long self, String fileName);
    private final native void N_SetToBytes(long self, int size, byte[] bytes);
    private final native Type N_GetType(long self);
    private final native String N_GetFile(long self);
    private final native int N_GetBytesCount(long self);
    private final native int N_GetBytes(long self, int size, byte[] bytes);
}
