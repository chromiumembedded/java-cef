// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.browser;

import java.awt.Canvas;
import java.awt.Component;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.Rectangle;
import java.awt.event.FocusEvent;
import java.awt.event.FocusListener;

import javax.swing.JPopupMenu;
import javax.swing.ToolTipManager;

import org.cef.OS;
import org.cef.handler.CefClientHandler;
import org.cef.handler.CefRenderHandler;

/**
 * This class represents a windowed rendered browser.
 * The visibility of this class is "package". To create a new 
 * CefBrowser instance, please use CefBrowserFactory.
 */
class CefBrowserWr extends CefBrowser_N {
  private Canvas canvas_ = null;
  private long window_handle_ = 0;
  private CefClientHandler clientHandler_;
  private String url_;

  @SuppressWarnings("serial")
  CefBrowserWr(CefClientHandler clientHandler, String url) {
    super();
    clientHandler_ = clientHandler;
    url_ = url;

    // Disabling lightweight of popup menu is required because
    // otherwise it will be displayed behind the content of canvas_
    JPopupMenu.setDefaultLightWeightPopupEnabled(false);
    ToolTipManager.sharedInstance().setLightWeightPopupEnabled(false);

    canvas_ = new Canvas() {
      @Override
      public void setBounds(int x, int y, int width, int height) {
        super.setBounds(x, y, width, height);
        wasResized(width, height);
      }

      @Override
      public void setBounds(Rectangle r) {
        setBounds(r.x, r.y, r.width, r.height);
      }

      @Override
      public void setSize(int width, int height) {
        super.setSize(width, height);
        wasResized(width, height);
      }

      @Override
      public void setSize(Dimension d) {
        setSize(d.width, d.height);
      }

      @Override
      public void paint(Graphics g) {
        if (getNativeRef("CefBrowser") == 0 ) {
          createBrowser(clientHandler_, getWindowHandle(), url_, false, canvas_);
        }
      }

      @Override
      public void removeNotify() {
        parentWindowWillClose();
        super.removeNotify();
      }
    };
    canvas_.setFocusable(true);
    canvas_.addFocusListener( new FocusListener() {
      @Override
      public void focusLost(FocusEvent e) {
        setFocus(false);
      }
      
      @Override
      public void focusGained(FocusEvent e) {
        setFocus(true);
      }
    });
  }

  @Override
  public Component getUIComponent() {
    return canvas_;
  }

  @Override
  public CefRenderHandler getRenderHandler() {
    return null;
  }

  private long getWindowHandle() {
    if (window_handle_ == 0 && OS.isMacintosh()) {
      try {
        Class<?> cls = Class.forName("org.cef.browser.mac.CefBrowserWindowMac");
        CefBrowserWindow browserWindow = (CefBrowserWindow)cls.newInstance();
        if (browserWindow != null) {
          window_handle_ = browserWindow.getWindowHandleOfCanvas(canvas_);
        }
      } catch (ClassNotFoundException e) {
        e.printStackTrace();
      } catch (InstantiationException e) {
        e.printStackTrace();
      } catch (IllegalAccessException e) {
        e.printStackTrace();
      }
    }
    return window_handle_;
  }
}
