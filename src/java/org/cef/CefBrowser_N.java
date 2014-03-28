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

  @Override
  public void setNativeRef(String identifer, long nativeRef) {
    N_CefHandle = nativeRef;
  }

  @Override
  public long getNativeRef(String identifer) {
    return N_CefHandle;
  }

  // Constructor is called by native code.
  CefBrowser_N() {
  }

  @Override
  protected void finalize() throws Throwable {
    close();
    super.finalize();
  }

  @Override
  public boolean canGoBack() {
    try {
      return N_CanGoBack();
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return false;
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
  public boolean canGoForward() {
    try {
      return N_CanGoForward();
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return false;
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
  public boolean isLoading() {
    try {
      return N_IsLoading();
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return false;
  }

  @Override
  public void reload() {
    try {
      N_Reload();
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
  }

  @Override
  public void reloadIgnoreCache() {
    try {
      N_ReloadIgnoreCache();
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    } 
  }

  @Override
  public void stopLoad() {
    try {
      N_StopLoad();
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
  public boolean isPopup() {
    try {
      return N_IsPopup();
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return false;
  }

  @Override
  public boolean hasDocument() {
    try {
      return N_HasDocument();
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return false;
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
  public void loadString(String val, String url) {
    try {
      N_LoadString(val, url);
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
  }

  @Override
  public void executeJavaScript(String code, String url, int line) {
    try {
      N_ExecuteJavaScript(code, url, line);
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
  }

  @Override
  public String getURL() {
    try {
      return N_GetURL();
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return "";
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
  public void setFocus(boolean enable) {
    try {
      N_SetFocus(enable);
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
  }

  @Override
  public double getZoomLevel() {
    try {
      return N_GetZoomLevel();
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return 0.0;
  }

  @Override
  public void setZoomLevel(double zoomLevel) {
    try {
      N_SetZoomLevel(zoomLevel);
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
  }

  @Override
  public void startDownload(String url) {
    try {
      N_StartDownload(url);
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
  }

  @Override
  public void showDevTools(CefClientHandler clientHandler) {
    try {
      N_ShowDevTools(clientHandler);
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
  }

  @Override
  public void closeDevTools() {
    try {
      N_CloseDevTools();
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
  }

  @Override
  public void wasResized(int width, int height) {
    try {
      N_WasResized(width, height);
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

  private final native boolean N_CanGoBack();
  private final native void N_GoBack();
  private final native boolean N_CanGoForward();
  private final native void N_GoForward();
  private final native boolean N_IsLoading();
  private final native void N_Reload();
  private final native void N_ReloadIgnoreCache();
  private final native void N_StopLoad();
  private final native int N_GetIdentifier();
  private final native boolean N_IsPopup();
  private final native boolean N_HasDocument();
  private final native void N_LoadURL(String url);
  private final native void N_LoadString(String val, String url);
  private final native void N_ExecuteJavaScript(String code, String url, int line);
  private final native String N_GetURL();
  private final native void N_Close();
  private final native void N_SetFocus(boolean enable);
  private final native double N_GetZoomLevel();
  private final native void N_SetZoomLevel(double zoomLevel);
  private final native void N_StartDownload(String url);
  private final native void N_ShowDevTools(CefClientHandler clientHandler);
  private final native void N_CloseDevTools();
  private final native void N_WasResized(int width, int height);
  private final native void N_Invalidate(Rectangle rect);
  private final native void N_SendKeyEvent(KeyEvent e);
  private final native void N_SendMouseEvent(MouseEvent e);
  private final native void N_SendMouseWheelEvent(MouseWheelEvent e);
}
