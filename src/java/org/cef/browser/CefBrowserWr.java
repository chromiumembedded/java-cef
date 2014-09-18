// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.browser;

import java.awt.BorderLayout;
import java.awt.Canvas;
import java.awt.Component;
import java.awt.Container;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.Point;
import java.awt.Rectangle;
import java.awt.Window;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.FocusEvent;
import java.awt.event.FocusListener;
import java.awt.event.HierarchyBoundsListener;
import java.awt.event.HierarchyEvent;
import java.awt.event.HierarchyListener;

import javax.swing.JPanel;
import javax.swing.JPopupMenu;
import javax.swing.MenuSelectionManager;
import javax.swing.SwingUtilities;
import javax.swing.Timer;
import javax.swing.ToolTipManager;

import org.cef.OS;
import org.cef.handler.CefClientHandler;
import org.cef.handler.CefRenderHandler;
import org.cef.handler.CefRenderHandlerAdapter;

/**
 * This class represents a windowed rendered browser.
 * The visibility of this class is "package". To create a new 
 * CefBrowser instance, please use CefBrowserFactory.
 */
class CefBrowserWr extends CefBrowser_N {
  private Canvas canvas_ = null;
  private Component component_ = null;
  private Rectangle content_rect_ = new Rectangle(0,0,0,0);
  private long window_handle_ = 0;
  private CefClientHandler clientHandler_;
  private String url_;
  private CefRequestContext context_;
  private CefBrowserWr parent_ = null;
  private CefBrowserWr devTools_ = null;
  private boolean isDisposed = false;
  private Timer delayedUpdate_ = new Timer(100, new ActionListener() {
    @Override
    public void actionPerformed(ActionEvent e) {
      SwingUtilities.invokeLater(new Runnable() {
        @Override
        public void run() {
          createUIIfRequired();
          if (OS.isMacintosh())
            doUpdate();
        }
      });
    }
  });
  private CefRenderHandlerAdapter render_handler_ = new CefRenderHandlerAdapter() {
    @Override
    public Rectangle getViewRect(CefBrowser browser) {
      synchronized (content_rect_) {
        return content_rect_;
      }
    }
  };

  CefBrowserWr(CefClientHandler clientHandler,
               String url,
               CefRequestContext context) {
    this(clientHandler, url, context, null);
  }

  @SuppressWarnings("serial")
  private CefBrowserWr(CefClientHandler clientHandler,
                       String url,
                       CefRequestContext context,
                       CefBrowserWr parent) {
    super();
    clientHandler_ = clientHandler;
    url_ = url;
    context_ = context;
    parent_ = parent;
    delayedUpdate_.setRepeats(false);

    // Disabling lightweight of popup menu is required because
    // otherwise it will be displayed behind the content of component_
    JPopupMenu.setDefaultLightWeightPopupEnabled(false);
    ToolTipManager.sharedInstance().setLightWeightPopupEnabled(false);

    // We're using a JComponent instead of a Canvas now because the
    // JComponent has clipping informations, which aren't accessible for Canvas.
    component_ = new JPanel(new BorderLayout()) {
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
        // If the user resizes the UI component, the new size and clipping
        // informations are forwarded to the native code.
        // But on Mac the last resize information doesn't resize the native UI
        // accurately (sometimes the native UI is too small). An easy way to
        // solve this, is to send the last Update-Information again. Therefore
        // we're setting up a delayedUpdate timer which is reset each time 
        // paint is called. This prevents the us of sending the UI update too
        // often.
        doUpdate();
        delayedUpdate_.restart();
      }
    };
    // On windows we have to use a Canvas because its a heavyweight component
    // and we need its native HWND as parent for the browser UI.
    if (OS.isWindows()) {
      canvas_ = new Canvas();
      ((JPanel)component_).add(canvas_, BorderLayout.CENTER);
    }

    // Initial minimal size of the component. Otherwise the UI won't work
    // accordingly in panes like JSplitPane.
    component_.setMinimumSize(new Dimension(0,0));
    component_.setFocusable(true);
    component_.addFocusListener( new FocusListener() {
      @Override
      public void focusLost(FocusEvent e) {
        setFocus(false);
      }

      @Override
      public void focusGained(FocusEvent e) {
        // Dismiss any Java menus that are currently displayed.
        MenuSelectionManager.defaultManager().clearSelectedPath();
        setFocus(true);
      }
    });
    component_.addHierarchyBoundsListener(new HierarchyBoundsListener() {
      @Override
      public void ancestorResized(HierarchyEvent e) {
        doUpdate();
      }
      @Override
      public void ancestorMoved(HierarchyEvent e) {
        doUpdate();
      }
    });
    component_.addHierarchyListener(new HierarchyListener() {
      @Override
      public void hierarchyChanged(HierarchyEvent e) {
        if( (e.getChangeFlags() & HierarchyEvent.SHOWING_CHANGED) != 0 ) {
          setWindowVisibility(e.getChanged().isVisible());
        }
      }
    });
  }

  @Override
  public Component getUIComponent() {
    return component_;
  }

  @Override
  public CefRenderHandler getRenderHandler() {
    return render_handler_;
  }

  @Override
  public synchronized void close() {
    isDisposed = true;
    if (context_ != null)
      context_.dispose();
    if (parent_ != null) {
      parent_.closeDevTools();
      parent_.devTools_ = null;
      parent_ = null;
    }
    super.close();
  }

  @Override
  public synchronized CefBrowser getDevTools() {
    if (devTools_ == null)
      devTools_ = new CefBrowserWr(clientHandler_, url_, context_, this);
    return devTools_;
  }

  private long getWindowHandle() {
    if (window_handle_ == 0 && OS.isMacintosh()) {
      try {
        Class<?> cls = Class.forName("org.cef.browser.mac.CefBrowserWindowMac");
        CefBrowserWindow browserWindow = (CefBrowserWindow)cls.newInstance();
        if (browserWindow != null) {
          window_handle_ = browserWindow.getWindowHandle(component_);
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

  private void doUpdate() {
    if (isDisposed)
      return;

    Rectangle clipping = ((JPanel)component_).getVisibleRect();

    if (OS.isMacintosh()) {
      Container parent = component_.getParent();
      Point contentPos = component_.getLocation();
      while (parent != null) {
        Container next = parent.getParent();
        if (next != null && next instanceof Window)
          break;
        Point parentPos = parent.getLocation();
        contentPos.translate(parentPos.x, parentPos.y);
        parent = next;
      }
      contentPos.translate(clipping.x, clipping.y);

      Point browserPos = clipping.getLocation();
      browserPos.x *=-1;
      browserPos.y *=-1;

      synchronized (content_rect_) {
        content_rect_ = new Rectangle(contentPos, clipping.getSize());
        Rectangle browserRect = new Rectangle(browserPos, component_.getSize());
        updateUI(content_rect_, browserRect);
      }
    } else {
      synchronized (content_rect_) {
        content_rect_ = component_.getBounds();
        updateUI(clipping, content_rect_);
      }
    }
  }

  private void createUIIfRequired() {
    if (getNativeRef("CefBrowser") == 0 && !isDisposed) {
      if (parent_ != null) {
        createDevTools(parent_,
                       clientHandler_,
                       getWindowHandle(),
                       false,
                       OS.isWindows() ? canvas_ : component_);
      } else {
        createBrowser(clientHandler_,
                      getWindowHandle(),
                      url_,
                      false,
                      OS.isWindows() ? canvas_ : component_,
                      context_);
      }
    }
  }
}
