// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.callback;

class CefDownloadItemCallback_N extends CefNativeAdapter implements CefDownloadItemCallback {
    CefDownloadItemCallback_N() {}

    @Override
    protected void finalize() throws Throwable {
        try {
            N_Dispose(getNativeRef(null));
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
        super.finalize();
    }

    @Override
    public void cancel() {
        try {
            N_Cancel(getNativeRef(null));
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
    }

    @Override
    public void pause() {
        try {
            N_Pause(getNativeRef(null));
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
    }

    @Override
    public void resume() {
        try {
            N_Resume(getNativeRef(null));
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
    }

    private final native void N_Dispose(long self);
    private final native void N_Cancel(long self);
    private final native void N_Pause(long self);
    private final native void N_Resume(long self);
}
