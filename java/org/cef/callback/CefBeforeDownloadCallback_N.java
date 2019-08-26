// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.callback;

class CefBeforeDownloadCallback_N extends CefNativeAdapter implements CefBeforeDownloadCallback {
    CefBeforeDownloadCallback_N() {}

    @Override
    protected void finalize() throws Throwable {
        Continue("", false);
        super.finalize();
    }

    @Override
    public void Continue(String downloadPath, boolean showDialog) {
        try {
            N_Continue(getNativeRef(null), downloadPath, showDialog);
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
    }

    private final native void N_Continue(long self, String downloadPath, boolean showDialog);
}
