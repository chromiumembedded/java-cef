// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.browser.mac;

import java.awt.Canvas;
import java.awt.Component;
import java.awt.peer.ComponentPeer;
import sun.lwawt.LWComponentPeer;
import sun.lwawt.PlatformWindow;
import sun.lwawt.macosx.CPlatformWindow;

import org.cef.browser.CefBrowserWindow;

public class CefBrowserWindowMac implements CefBrowserWindow {
  @Override
  public long getWindowHandleOfCanvas(Canvas canvas) {
    long result = 0;
    Component comp = canvas;
    while (comp != null) {
      if (comp.isLightweight()) {
        comp = comp.getParent();
        continue;
      }
      @SuppressWarnings("deprecation")
      ComponentPeer peer = comp.getPeer();
      if (peer instanceof LWComponentPeer) {
        @SuppressWarnings("rawtypes")
        PlatformWindow pWindow = ((LWComponentPeer) peer).getPlatformWindow();
        if (pWindow instanceof CPlatformWindow) {
          result = ((CPlatformWindow)pWindow).getNSWindowPtr();
          break;
        }
      }
      comp = comp.getParent();
    }
    return result;
  }
}
