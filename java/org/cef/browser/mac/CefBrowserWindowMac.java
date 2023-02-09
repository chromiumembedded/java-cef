// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.browser.mac;

import org.cef.browser.CefBrowserWindow;

import java.awt.*;

public class CefBrowserWindowMac implements CefBrowserWindow {
    @Override
    public long getWindowHandle(Component comp) {
        return 0;
    }
}
