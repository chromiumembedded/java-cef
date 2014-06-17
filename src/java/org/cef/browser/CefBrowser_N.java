// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.browser;

import java.awt.event.KeyEvent;
import java.awt.event.MouseEvent;
import java.awt.event.MouseWheelEvent;
import java.awt.Canvas;
import java.awt.Rectangle;
import java.util.Vector;

import org.cef.callback.CefNativeAdapter;
import org.cef.callback.CefRunFileDialogCallback;
import org.cef.callback.CefStringVisitor;
import org.cef.handler.CefClientHandler;
import org.cef.handler.CefDialogHandler.FileDialogMode;
import org.cef.network.CefRequest;

/**
 * This class represents all methods which are connected to the
 * native counterpart CEF.
 * The visibility of this class is "package". To create a new
 * CefBrowser instance, please use CefBrowserFactory.
 */
abstract class CefBrowser_N extends CefNativeAdapter implements CefBrowser {
  private boolean isPending_ = false;

  /**
   * Create a new browser.
   */
  protected void createBrowser(CefClientHandler clientHandler,
                               long windowHandle,
                               String url,
                               boolean transparent,
                               Canvas canvas,
                               CefRequestContext context) {
    if (getNativeRef("CefBrowser") == 0 && !isPending_) {
      try {
        isPending_ = N_CreateBrowser(clientHandler,
                                     windowHandle,
                                     url,
                                     transparent,
                                     canvas,
                                     context);
      } catch (UnsatisfiedLinkError err) {
        err.printStackTrace();
      }
    }
  }

  /**
   * Create a new browser as dev tools
   */
  protected final void createDevTools(CefBrowser parent,
                                      CefClientHandler clientHandler,
                                      long windowHandle,
                                      boolean transparent,
                                      Canvas canvas) {
    if (getNativeRef("CefBrowser") == 0 && !isPending_) {
      try {
        isPending_ = N_CreateDevTools(parent,
                                      clientHandler,
                                      windowHandle,
                                      transparent,
                                      canvas);
      } catch (UnsatisfiedLinkError err) {
        err.printStackTrace();
      }
    }
  }

  /**
   * Returns the native window handle for the specified native surface handle.
   */
  protected final long getWindowHandle(long surfaceHandle) {
    try {
      return N_GetWindowHandle(surfaceHandle);
    } catch (UnsatisfiedLinkError err) {
      err.printStackTrace();
    }
    return 0;
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

  public void viewSource() {
    try {
      N_ViewSource();
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
  }

  public void getSource(CefStringVisitor visitor) {
    try {
      N_GetSource(visitor);
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
  }

  public void getText(CefStringVisitor visitor) {
    try {
      N_GetText(visitor);
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
  }

  @Override
  public void loadRequest(CefRequest request) {
    try {
      N_LoadRequest(request);
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
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
  public void runFileDialog(FileDialogMode mode,
                            String title,
                            String defaultFileName,
                            Vector<String> acceptTypes,
                            CefRunFileDialogCallback callback) {
    try {
      N_RunFileDialog(mode, title, defaultFileName, acceptTypes, callback);
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
  public void print() {
    try {
      N_Print();
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
  }

  @Override
  public void find(int identifier,
                   String searchText,
                   boolean forward,
                   boolean matchCase,
                   boolean findNext) {
    try {
      N_Find(identifier, searchText, forward, matchCase, findNext);
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
  }

  @Override
  public void stopFinding(boolean clearSelection) {
    try {
      N_StopFinding(clearSelection);
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
  }

  protected final void closeDevTools() {
    try {
      N_CloseDevTools();
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
  }

  /**
   * Notify CEF that the parent window will close.
   */
  protected final void parentWindowWillClose() {
    try {
      N_ParentWindowWillClose();
    } catch(UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
  }

  /**
  * Notify that the browser was resized.
  * @param width The new width of the browser
  * @param height The new height of the browser
  */
  protected final void wasResized(int width, int height) {
    try {
      N_WasResized(width, height);
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
  }

  /**
  * Invalidate the specified rectangle.
  * @param rect The rectangle to invalidate.
  */
  protected final void invalidate(Rectangle rect) {
    try {
      N_Invalidate(rect);
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
  }

  /**
  * Send a key event.
  * @param e The event to send.
  */
  protected final void sendKeyEvent(KeyEvent e) {
    try {
      N_SendKeyEvent(e);
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
  }

  /**
   * Send a mouse event.
   * @param e The event to send.
   */
  protected final void sendMouseEvent(MouseEvent e) {
    try {
      N_SendMouseEvent(e);
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
  }

  /**
   * Send a mouse wheel event.
   * @param e The event to send.
   */
  protected final void sendMouseWheelEvent(MouseWheelEvent e) {
    try {
      N_SendMouseWheelEvent(e);
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
  }

  private final native boolean N_CreateBrowser(CefClientHandler clientHandler,
                                               long windowHandle,
                                               String url,
                                               boolean transparent,
                                               Canvas canvas,
                                               CefRequestContext context);
  private final native boolean N_CreateDevTools(CefBrowser parent,
                                                CefClientHandler clientHandler,
                                                long windowHandle,
                                                boolean transparent,
                                                Canvas canvas);
  private final native long N_GetWindowHandle(long surfaceHandle);
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
  private final native void N_ViewSource();
  private final native void N_GetSource(CefStringVisitor visitor);
  private final native void N_GetText(CefStringVisitor visitor);
  private final native void N_LoadRequest(CefRequest request);
  private final native void N_LoadURL(String url);
  private final native void N_LoadString(String val, String url);
  private final native void N_ExecuteJavaScript(String code, String url, int line);
  private final native String N_GetURL();
  private final native void N_ParentWindowWillClose();
  private final native void N_Close();
  private final native void N_SetFocus(boolean enable);
  private final native double N_GetZoomLevel();
  private final native void N_SetZoomLevel(double zoomLevel);
  private final native void N_RunFileDialog(FileDialogMode mode, String title, String defaultFileName, Vector<String> acceptTypes, CefRunFileDialogCallback callback);
  private final native void N_StartDownload(String url);
  private final native void N_Print();
  private final native void N_Find(int identifier, String searchText, boolean forward, boolean matchCase, boolean findNext);
  private final native void N_StopFinding(boolean clearSelection);
  private final native void N_CloseDevTools();
  private final native void N_WasResized(int width, int height);
  private final native void N_Invalidate(Rectangle rect);
  private final native void N_SendKeyEvent(KeyEvent e);
  private final native void N_SendMouseEvent(MouseEvent e);
  private final native void N_SendMouseWheelEvent(MouseWheelEvent e);
}
