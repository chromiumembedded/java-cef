// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.callback;

class CefCallback_N extends CefNativeAdapter implements CefCallback {
    CefCallback_N() {}

    @Override
    protected void finalize() throws Throwable {
        cancel();
        super.finalize();
    }

    @Override
    public void Continue() {
        try {
            N_Continue(getNativeRef(null));
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
    }

    @Override
    public void cancel() {
        try {
            N_Cancel(getNativeRef(null));
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
    }

    private final native void N_Continue(long self);
    private final native void N_Cancel(long self);
}
