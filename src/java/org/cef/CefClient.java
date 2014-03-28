// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef;

import java.awt.Cursor;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.event.MouseMotionListener;
import java.awt.event.MouseWheelEvent;
import java.awt.event.MouseWheelListener;
import java.awt.Canvas;
import java.awt.Container;
import java.awt.Dimension;
import java.awt.Point;
import java.awt.Rectangle;
import java.awt.Window;
import java.nio.ByteBuffer;
import javax.media.nativewindow.NativeSurface;
import javax.media.opengl.awt.GLCanvas;
import javax.media.opengl.GLAutoDrawable;
import javax.media.opengl.GLEventListener;
import javax.media.opengl.GLProfile;
import javax.media.opengl.GLCapabilities;
import javax.swing.JPopupMenu;
import javax.swing.ToolTipManager;

/**
 * Client that owns a browser and renderer.
 */
public class CefClient extends CefClientHandler implements CefLifeSpanHandler,
                                                           CefDisplayHandler,
                                                           CefRenderHandler,
                                                           CefFocusHandler,
                                                           CefMessageRouterHandler,
                                                           CefLoadHandler {
  private boolean osr_enabled_;
  private boolean isTransparent_;
  private CefRenderer renderer_;
  private Canvas canvas_;
  private long window_handle_ = 0;
  private CefBrowser browser_ = null;
  private Rectangle browser_rect_ = new Rectangle(0, 0, 0, 0);
  private CefDisplayHandler displayHandler_ = null;
  private CefFocusHandler focusHandler_ = null;
  private CefLifeSpanHandler lifeSpanHandler_ = null;
  private CefLoadHandler loadHandler_ = null;
  private CefMessageRouterHandler msgRouterHandler_ = null;

  public CefClient(boolean transparent, boolean osr) {
    super();
    isTransparent_ = transparent;
    osr_enabled_ = osr;
    if (osr_enabled_) {
      renderer_ = new CefRenderer(transparent);
      createGLCanvas();
    } else {
      // Disabling lightweight of popup menu is required because
      // otherwise it will be displayed behind the content of canvas_
      JPopupMenu.setDefaultLightWeightPopupEnabled(false);
      ToolTipManager.sharedInstance().setLightWeightPopupEnabled(false);

      renderer_ = null;
      canvas_ = new Canvas() {
        @Override
        public void setBounds(int x, int y, int width, int height) {
          super.setBounds(x, y, width, height);
          if (browser_ != null) {
            browser_.wasResized(width, height);
          }
        }

        @Override
        public void setBounds(Rectangle r) {
          setBounds(r.x, r.y, r.width, r.height);
        }

        @Override
        public void setSize(int width, int height) {
          super.setSize(width, height);
          if (browser_ != null)
            browser_.wasResized(width, height);
        }

        @Override
        public void setSize(Dimension d) {
          setSize(d.width, d.height);
        }
      };
    }
  }
  
  @Override
  protected void finalize() throws Throwable {
    if (browser_ != null)
      destroyBrowser();
    removeDisplayHandler(this);
    removeFocusHandler(this);
    removeLifeSpanHandler(this);
    removeMessageRouterHandler(this);
    removeLoadHandler(this);
    removeRenderHandler(this);
    super.finalize();
  }

  public void createBrowser(String url) {
    assert(browser_ == null);
    browser_ = CefContext.createBrowser(this, getWindowHandle(), url, isTransparent_, (osr_enabled_ ? null : canvas_));
  }

  public void destroyBrowser() {
    assert(browser_ != null);
    browser_.close();
    browser_ = null;
  }

  public CefBrowser getBrowser() {
    return browser_;
  }

  public CefClient addDisplayHandler(CefDisplayHandler handler) {
    if (displayHandler_ == null)
      displayHandler_ = handler;
    return this;
  }

  public void removeDisplayHandler() {
    displayHandler_ = null;
  }

  public CefClient addFocusHandler(CefFocusHandler handler) {
    if (focusHandler_ == null)
      focusHandler_ = handler;
    return this;
  }

  public void removeFocusHandler() {
    focusHandler_ = null;
  }

  public CefClient addLifeSpanHandler(CefLifeSpanHandler handler) {
    if (lifeSpanHandler_ == null)
      lifeSpanHandler_ = handler;
    return this;
  }

  public void removeLifeSpanHandler() {
    lifeSpanHandler_ = null;
  }

  public CefClient addMessageRouterHandler(CefMessageRouterHandler handler) {
    if (msgRouterHandler_ == null)
      msgRouterHandler_ = handler;
    return this;
  }

  public void removeMessageRouterHandler() {
    msgRouterHandler_ = null;
  }

  public CefClient addLoadHandler(CefLoadHandler handler) {
    if (loadHandler_ == null)
      loadHandler_ = handler;
    return this;
  }

  public void removeLoadHandler() {
    loadHandler_ = null;
  }

  @Override
  protected CefDisplayHandler getDisplayHandler() {
    return this;
  }

  @Override
  protected CefFocusHandler getFocusHandler() {
    return this;
  }

  @Override
  protected CefLifeSpanHandler getLifeSpanHandler() {
    return this;
  }

  @Override
  protected CefMessageRouterHandler getMessageRouterHandler() {
    return this;
  }

  @Override
  protected CefLoadHandler getLoadHandler() {
    return this;
  }

  @Override
  protected CefRenderHandler getRenderHandler() {
    return this;
  }

  public Canvas getCanvas() {
    return canvas_;
  }

  private long getWindowHandle() {
    if (window_handle_ == 0) {
      if (osr_enabled_) {
        NativeSurface surface = ((GLCanvas)canvas_).getNativeSurface();
        surface.lockSurface();
        window_handle_ = CefContext.getWindowHandle(surface.getSurfaceHandle());
        surface.unlockSurface();
        assert (window_handle_ != 0);
      } else if (CefContext.isMacintosh()) {
        try {
          Class<?> cls = Class.forName("org.cef.mac.CefBrowserWindowMac");
          CefBrowserWindow browserWindow = (CefBrowserWindow)cls.newInstance();
          if (browserWindow != null) {
            window_handle_ = browserWindow.getWindowHandleOfCanvas(canvas_);
          }
        } catch (ClassNotFoundException e) {
          // Ignore exceptions
        } catch (InstantiationException e) {
          // Ignore exceptions
        } catch (IllegalAccessException e) {
          // Ignore exceptions
        }
      }
    }
    return window_handle_;
  }

  private void createGLCanvas() {
    GLProfile glprofile = GLProfile.getDefault();
    GLCapabilities glcapabilities = new GLCapabilities(glprofile);
    canvas_ = new GLCanvas(glcapabilities);

    ((GLCanvas)canvas_).addGLEventListener(new GLEventListener() {
      @Override
      public void reshape(GLAutoDrawable glautodrawable, int x, int y, int width, int height) {
        browser_rect_.setBounds(x, y, width, height);
        if (browser_ != null)
          browser_.wasResized(width, height);
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
        onMouseEvent(e);
      }

      @Override
      public void mouseReleased(MouseEvent e) {
        onMouseEvent(e);
      }

      @Override
      public void mouseEntered(MouseEvent e) {
        onMouseEvent(e);
      }

      @Override
      public void mouseExited(MouseEvent e) {
        onMouseEvent(e);
      }

      @Override
      public void mouseClicked(MouseEvent e) {
        onMouseEvent(e);
      }
    });

    canvas_.addMouseMotionListener(new MouseMotionListener() {
      @Override
      public void mouseMoved(MouseEvent e) {
        onMouseEvent(e);
      }

      @Override
      public void mouseDragged(MouseEvent e) {
        onMouseEvent(e);
      }
    });

    canvas_.addMouseWheelListener(new MouseWheelListener() {
      @Override
      public void mouseWheelMoved(MouseWheelEvent e) {
        onMouseWheelEvent(e);
      }
    });

    canvas_.addKeyListener(new KeyListener() {
      @Override
      public void keyTyped(KeyEvent e) {
        onKeyEvent(e);
      }

      @Override
      public void keyPressed(KeyEvent e) {
        onKeyEvent(e);
      }

      @Override
      public void keyReleased(KeyEvent e) {
        onKeyEvent(e);
      }
    });
  }

  private void onMouseEvent(MouseEvent e) {
    if (browser_ != null)
      browser_.sendMouseEvent(e);
  }
  
  private void onMouseWheelEvent(MouseWheelEvent e) {
    if (browser_ != null)
      browser_.sendMouseWheelEvent(e);
  }

  private void onKeyEvent(KeyEvent e) {
    if (browser_ != null)
      browser_.sendKeyEvent(e);
  }

  @Override
  public void onAfterCreated(CefBrowser browser) {
    if (lifeSpanHandler_ != null)
      lifeSpanHandler_.onAfterCreated(browser);
  }

  @Override
  public void onAddressChange(CefBrowser browser, String url) {
    if (displayHandler_ != null)
      displayHandler_.onAddressChange(browser, url);
  }

  @Override
  public void onTitleChange(CefBrowser browser, String title) {
    if (displayHandler_ != null)
      displayHandler_.onTitleChange(browser, title);
  }

  @Override
  public Rectangle getViewRect(CefBrowser browser) {
    if (osr_enabled_)
      return browser_rect_;

    Rectangle tmp = canvas_.getBounds();
    if (CefContext.isMacintosh()) {
      Container parent = canvas_.getParent();
      while (parent != null) {
        Container next = parent.getParent();
        if (next != null && next instanceof Window)
          break;

        Rectangle parentRect = parent.getBounds();
        tmp.x += parentRect.x;
        tmp.y += parentRect.y;
        parent = next;
      }
    }
    return tmp;
  }

  @Override
  public Point getScreenPoint(CefBrowser browser, Point viewPoint) {
    Point screenPoint = canvas_.getLocationOnScreen();
    screenPoint.translate(viewPoint.x, viewPoint.y);
    return screenPoint;
  }

  @Override
  public void onPopupShow(CefBrowser browser,
                          boolean show) {
    if (!show) {
      Rectangle old_rect = renderer_.getPopupRect();
      renderer_.clearPopupRects();
      browser_.invalidate(old_rect);
    }
  }

  @Override
  public void onPopupSize(CefBrowser browser,
                          Rectangle size) {
    renderer_.onPopupSize(size);
  }

  @Override
  public void onPaint(CefBrowser browser,
                      boolean popup,
                      Rectangle[] dirtyRects,
                      ByteBuffer buffer,
                      int width,
                      int height) {
    ((GLCanvas)canvas_).getContext().makeCurrent();
    renderer_.onPaint(((GLCanvas)canvas_).getGL().getGL2(), popup, dirtyRects, buffer, width, height);
    ((GLCanvas)canvas_).getContext().release();
    ((GLCanvas)canvas_).display();
  }

  @Override
  public void onCursorChange(CefBrowser browser,
                             int cursorType) {
    if (browser_ != null)
      getCanvas().setCursor(new Cursor(cursorType));
  }

  @Override
  public void onTakeFocus(CefBrowser browser, boolean next) {
    browser.setFocus(false);
    if (focusHandler_ != null)
      focusHandler_.onTakeFocus(browser, next);
  }

  @Override
  public boolean onSetFocus(CefBrowser browser, FocusSource source) {
    boolean alreadyHandled = false;
    if (focusHandler_ != null)
      alreadyHandled = focusHandler_.onSetFocus(browser, source);
    if (!alreadyHandled && canvas_ != null)
      canvas_.requestFocus();
    return alreadyHandled;
  }

  @Override
  public void onGotFocus(CefBrowser browser) {
    if (focusHandler_ != null)
      focusHandler_.onGotFocus(browser);
  }

  @Override
  public boolean onQuery(CefBrowser browser,
                         long query_id,
                         String request,
                         boolean persistent,
                         CefQueryCallback callback) {
    boolean alreadyHandled = false;
    if (msgRouterHandler_ != null)
      alreadyHandled = msgRouterHandler_.onQuery(browser, query_id, request, persistent, callback);
    return alreadyHandled;
  }

  @Override
  public void onQueryCanceled(CefBrowser browser,
                              long query_id) {
    if (msgRouterHandler_ != null)
      msgRouterHandler_.onQueryCanceled(browser, query_id);
  }

  @Override
  public void onLoadingStateChange(CefBrowser browser,
                                   boolean isLoading,
                                   boolean canGoBack,
                                   boolean canGoForward) {
    if (loadHandler_ != null)
      loadHandler_.onLoadingStateChange(browser, isLoading, canGoBack, canGoForward);
  }

  @Override
  public void onLoadStart(CefBrowser browser, int frameIdentifer) {
    if (loadHandler_ != null)
      loadHandler_.onLoadStart(browser, frameIdentifer);
  }

  @Override
  public void onLoadEnd(CefBrowser browser,
                        int frameIdentifier,
                        int httpStatusCode) {
    if (loadHandler_ != null)
      loadHandler_.onLoadEnd(browser, frameIdentifier, httpStatusCode);
  }

  @Override
  public void onLoadError(CefBrowser browser,
                          int frameIdentifer,
                          ErrorCode errorCode,
                          String errorText,
                          String failedUrl) {
    if (loadHandler_ != null)
      loadHandler_.onLoadError(browser, frameIdentifer, errorCode, errorText, failedUrl);
  }
}
