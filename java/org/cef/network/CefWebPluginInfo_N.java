// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.network;

import org.cef.callback.CefNativeAdapter;

class CefWebPluginInfo_N extends CefNativeAdapter implements CefWebPluginInfo {
    CefWebPluginInfo_N() {}

    @Override
    public String getName() {
        try {
            return N_GetName(getNativeRef(null));
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
        return null;
    }

    @Override
    public String getPath() {
        try {
            return N_GetPath(getNativeRef(null));
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
        return null;
    }

    @Override
    public String getVersion() {
        try {
            return N_GetVersion(getNativeRef(null));
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
        return null;
    }

    @Override
    public String getDescription() {
        try {
            return N_GetDescription(getNativeRef(null));
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
        return null;
    }

    private final native String N_GetName(long self);
    private final native String N_GetPath(long self);
    private final native String N_GetVersion(long self);
    private final native String N_GetDescription(long self);
}
