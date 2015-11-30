// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.browser;

import java.awt.Component;
import java.awt.Cursor;
import java.awt.Graphics;
import java.awt.event.FocusEvent;
import java.awt.event.FocusListener;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.event.MouseMotionListener;
import java.awt.event.MouseWheelEvent;
import java.awt.event.MouseWheelListener;
import java.awt.Point;
import java.awt.Rectangle;
import java.nio.ByteBuffer;

import javax.media.nativewindow.NativeSurface;
import javax.media.opengl.awt.GLCanvas;
import javax.media.opengl.GLAutoDrawable;
import javax.media.opengl.GLEventListener;
import javax.media.opengl.GLProfile;
import javax.media.opengl.GLCapabilities;
import javax.swing.MenuSelectionManager;
import javax.swing.SwingUtilities;

import org.cef.callback.CefDragData;
import org.cef.handler.CefClientHandler;
import org.cef.handler.CefRenderHandler;

/**
 * This class represents an off-screen rendered browser.
 * The visibility of this class is "package". To create a new 
 * CefBrowser instance, please use CefBrowserFactory.
 */
class CefBrowserOsr extends CefBrowser_N implements CefRenderHandler {
  private CefRenderer renderer_;
  private GLCanvas canvas_;
  private long window_handle_ = 0;
  private Rectangle browser_rect_ = new Rectangle(0, 0, 1, 1);  // Work around CEF issue #1437.
  private Point screenPoint_ = new Point(0, 0);
  private CefClientHandler clientHandler_;
  private String url_;
  private boolean isTransparent_;
  private CefRequestContext context_;
  private CefBrowserOsr parent_ = null;
  private Point inspectAt_ = null;
  private CefBrowserOsr devTools_ = null;;

  CefBrowserOsr(CefClientHandler clientHandler,
                String url,
                boolean transparent,
                CefRequestContext context) {
    this(clientHandler, url, transparent, context, null, null);
  }

  private CefBrowserOsr(CefClientHandler clientHandler,
                        String url,
                        boolean transparent,
                        CefRequestContext context,
                        CefBrowserOsr parent,
                        Point inspectAt) {
    super();
    isTransparent_ = transparent;
    renderer_ = new CefRenderer(transparent);
    clientHandler_ = clientHandler;
    url_ = url;
    context_ = context;
    parent_ = parent;
    inspectAt_ = inspectAt;
    createGLCanvas();
  }

  @Override
  public Component getUIComponent() {
    return canvas_;
  }

  @Override
  public CefRenderHandler getRenderHandler() {
    return this;
  }

  @Override
  public synchronized void close() {
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
    return getDevTools(null);
  }

  @Override
  public synchronized CefBrowser getDevTools(Point inspectAt) {
    if (devTools_ == null) {
      devTools_ = new CefBrowserOsr(clientHandler_,
                                    url_,
                                    isTransparent_,
                                    context_,
                                    this,
                                    inspectAt);
    }
    return devTools_;
  }

  private long getWindowHandle() {
    if (window_handle_ == 0) {
      NativeSurface surface = canvas_.getNativeSurface();
      if (surface != null) {
        surface.lockSurface();
        window_handle_ = getWindowHandle(surface.getSurfaceHandle());
        surface.unlockSurface();
        assert (window_handle_ != 0);
      }
    }
    return window_handle_;
  }

  @SuppressWarnings("serial")
  private void createGLCanvas() {
    GLProfile glprofile = GLProfile.getMaxFixedFunc(true);
    GLCapabilities glcapabilities = new GLCapabilities(glprofile);
    canvas_ = new GLCanvas(glcapabilities) {
      @Override
      public void paint(Graphics g) {
        if (parent_ != null) {
          createDevTools(parent_,
                         clientHandler_,
                         getWindowHandle(),
                         isTransparent_,
                         null,
                         inspectAt_);
        } else {
          createBrowser(clientHandler_,
                        getWindowHandle(),
                        url_,
                        isTransparent_,
                        null,
                        context_);
        }
        super.paint(g);
      }
    };

    canvas_.addGLEventListener(new GLEventListener() {
      @Override
      public void reshape(GLAutoDrawable glautodrawable, int x, int y, int width, int height) {
        browser_rect_.setBounds(x, y, width, height);
        screenPoint_ = canvas_.getLocationOnScreen();
        wasResized(width, height);
      }

      @Override
      public void init(GLAutoDrawable glautodrawable) {
        renderer_.initialize(glautodrawable.getGL().getGL2());
      }

      @Override
      public void dispose(GLAutoDrawable glautodrawable) {
        renderer_.cleanup(glautodrawable.getGL().getGL2());
      }

      @Override
      public void display(GLAutoDrawable glautodrawable) {
        renderer_.render(glautodrawable.getGL().getGL2());
      }
    });

    canvas_.addMouseListener(new MouseListener() {
      @Override
      public void mousePressed(MouseEvent e) {
        sendMouseEvent(e);
      }

      @Override
      public void mouseReleased(MouseEvent e) {
        sendMouseEvent(e);
      }

      @Override
      public void mouseEntered(MouseEvent e) {
        sendMouseEvent(e);
      }

      @Override
      public void mouseExited(MouseEvent e) {
        sendMouseEvent(e);
      }

      @Override
      public void mouseClicked(MouseEvent e) {
        sendMouseEvent(e);
      }
    });

    canvas_.addMouseMotionListener(new MouseMotionListener() {
      @Override
      public void mouseMoved(MouseEvent e) {
        sendMouseEvent(e);
      }

      @Override
      public void mouseDragged(MouseEvent e) {
        sendMouseEvent(e);
      }
    });

    canvas_.addMouseWheelListener(new MouseWheelListener() {
      @Override
      public void mouseWheelMoved(MouseWheelEvent e) {
        sendMouseWheelEvent(e);
      }
    });

    canvas_.addKeyListener(new KeyListener() {
      @Override
      public void keyTyped(KeyEvent e) {
        sendKeyEvent(e);
      }

      @Override
      public void keyPressed(KeyEvent e) {
        sendKeyEvent(e);
      }

      @Override
      public void keyReleased(KeyEvent e) {
        sendKeyEvent(e);
      }
    });

    canvas_.setFocusable(true);
    canvas_.addFocusListener(new FocusListener() {
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
  }

  @Override
  public Rectangle getViewRect(CefBrowser browser) {
    return browser_rect_;
  }

  @Override
  public Point getScreenPoint(CefBrowser browser, Point viewPoint) {
    Point screenPoint = new Point(screenPoint_);
    screenPoint.translate(viewPoint.x, viewPoint.y);
    return screenPoint;
  }

  @Override
  public void onPopupShow(CefBrowser browser, boolean show) {
    if (!show) {
      renderer_.clearPopupRects();
      invalidate();
     }
  }

  @Override
  public void onPopupSize(CefBrowser browser, Rectangle size) {
    renderer_.onPopupSize(size);
  }

  @Override
  public void onPaint(CefBrowser browser, 
                      boolean popup,
                      Rectangle[] dirtyRects, 
                      ByteBuffer buffer, 
                      int width, 
                      int height) {
    canvas_.getContext().makeCurrent();
    renderer_.onPaint(canvas_.getGL().getGL2(), popup, dirtyRects, buffer, width, height);
    canvas_.getContext().release();
    SwingUtilities.invokeLater(new Runnable() {
      public void run() {
        canvas_.display();
      }
    });
  }

  @Override
  public void onCursorChange(CefBrowser browser, final int cursorType) {
    SwingUtilities.invokeLater(new Runnable() {
      public void run() {
        canvas_.setCursor(new Cursor(cursorType));
      }
    });
  }

  @Override
  public boolean startDragging(CefBrowser browser,
                               CefDragData dragData,
                               int mask,
                               int x,
                               int y) {
    // TODO(JCEF) Prepared for DnD support using OSR mode.
    return false;
  }

  @Override
  public void updateDragCursor(CefBrowser browser, int operation) {
    // TODO(JCEF) Prepared for DnD support using OSR mode.
  }
}
