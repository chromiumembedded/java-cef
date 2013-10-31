// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef;

import java.awt.event.KeyEvent;
import java.awt.event.MouseEvent;
import java.awt.event.MouseWheelEvent;
import java.awt.Rectangle;

class CefBrowser_N implements CefBrowser {
  // Used internally to store a pointer to the CEF object.
  private long N_CefHandle = 0;

  // Constructor is called by native code.
  CefBrowser_N() {
  }

  @Override
  protected void finalize() throws Throwable {
    close();
    super.finalize();
  }
  
  @Override
  public void close() {
    try {
      N_Close();
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
  }

  @Override
  public void goBack() {
    try {
      N_GoBack();
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
  }

  @Override
  public void goForward() {
    try {
      N_GoForward();
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
  }
  
  @Override
  public int getIdentifier() {
    try {
      return N_GetIdentifier();
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
      return -1;
    }
  }
  
  @Override
  public void loadURL(String url) {
    try {
      N_LoadURL(url);
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
  }

  @Override
  public void wasResized() {
    try {
      N_WasResized();
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
  }

  @Override
  public void invalidate(Rectangle rect) {
    try {
      N_Invalidate(rect);
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
  }

  @Override
  public void sendKeyEvent(KeyEvent e) {
    try {
      N_SendKeyEvent(e);
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
  }

  @Override
  public void sendMouseEvent(MouseEvent e) {
    try {
      N_SendMouseEvent(e);
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
  }

  @Override
  public void sendMouseWheelEvent(MouseWheelEvent e) {
    try {
      N_SendMouseWheelEvent(e);
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
  }

  private final native void N_Close();
  private final native void N_GoBack();
  private final native void N_GoForward();
  private final native int N_GetIdentifier();
  private final native void N_LoadURL(String url);
  private final native void N_WasResized();
  private final native void N_Invalidate(Rectangle rect);
  private final native void N_SendKeyEvent(KeyEvent e);
  private final native void N_SendMouseEvent(MouseEvent e);
  private final native void N_SendMouseWheelEvent(MouseWheelEvent e);
}
