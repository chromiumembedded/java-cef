// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.callback;

import java.nio.ByteBuffer;

class CefQueryCallback_N extends CefNativeAdapter implements CefQueryCallback {
    CefQueryCallback_N() {}

    @Override
    protected void finalize() throws Throwable {
        failure(-1, "Unexpected call to CefQueryCallback_N::finalize()");
        super.finalize();
    }

    @Override
    public void success(String response) {
        try {
            N_Success(getNativeRef(null), response, isPersistent());
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
    }

    @Override
    public void success(ByteBuffer response) {
        try {
            N_SuccessBinary(getNativeRef(null), response, isPersistent());
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
    }

    @Override
    public void failure(int error_code, String error_message) {
        try {
            N_Failure(getNativeRef(null), error_code, error_message);
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
    }

    @Override
    public boolean isPersistent() {
        return false;
    }

    private final native void N_Success(long self, String response, boolean persistent);
    private final native void N_SuccessBinary(long self, ByteBuffer response, boolean persistent);
    private final native void N_Failure(long self, int error_code, String error_message);
}

class CefQueryCallback_N_Persistent extends CefQueryCallback_N {
    CefQueryCallback_N_Persistent() {}

    @Override
    public boolean isPersistent() {
        return true;
    }
}
