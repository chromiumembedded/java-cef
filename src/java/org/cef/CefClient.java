// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef;

import java.awt.Component;
import java.awt.Container;
import java.awt.FocusTraversalPolicy;
import java.awt.Point;
import java.awt.Rectangle;
import java.awt.Window;
import java.nio.ByteBuffer;
import java.util.Collection;
import java.util.HashMap;
import java.util.Vector;

import org.cef.browser.CefBrowser;
import org.cef.browser.CefBrowserFactory;
import org.cef.callback.CefBeforeDownloadCallback;
import org.cef.callback.CefContextMenuParams;
import org.cef.callback.CefDownloadItem;
import org.cef.callback.CefDownloadItemCallback;
import org.cef.callback.CefDragData;
import org.cef.callback.CefFileDialogCallback;
import org.cef.callback.CefMenuModel;
import org.cef.callback.CefQueryCallback;
import org.cef.handler.CefClientHandler;
import org.cef.handler.CefContextMenuHandler;
import org.cef.handler.CefDialogHandler;
import org.cef.handler.CefDisplayHandler;
import org.cef.handler.CefDownloadHandler;
import org.cef.handler.CefDragHandler;
import org.cef.handler.CefFocusHandler;
import org.cef.handler.CefLifeSpanHandler;
import org.cef.handler.CefLoadHandler;
import org.cef.handler.CefMessageRouterHandler;
import org.cef.handler.CefRenderHandler;

/**
 * Client that owns a browser and renderer.
 */
public class CefClient extends CefClientHandler implements CefContextMenuHandler,
                                                           CefDialogHandler,
                                                           CefDownloadHandler,
                                                           CefDragHandler,
                                                           CefLifeSpanHandler,
                                                           CefDisplayHandler,
                                                           CefRenderHandler,
                                                           CefFocusHandler,
                                                           CefMessageRouterHandler,
                                                           CefLoadHandler {
  private HashMap<Integer,CefBrowser> browser_ = new HashMap<Integer,CefBrowser>();
  private CefDisplayHandler displayHandler_ = null;
  private CefFocusHandler focusHandler_ = null;
  private CefLifeSpanHandler lifeSpanHandler_ = null;
  private CefLoadHandler loadHandler_ = null;
  private CefMessageRouterHandler msgRouterHandler_ = null;
  private CefDialogHandler dialogHandler_ = null;
  private CefDownloadHandler downloadHandler_ = null;
  private CefContextMenuHandler contextMenuHandler_ = null;
  private CefDragHandler dragHandler_ = null;

  /**
   * The CTOR is only accessible within this package.
   * Use CefApp.createClient() to create an instance of
   * this class.
   * @see org.cef.CefApp.createClient()
   */
  CefClient() throws UnsatisfiedLinkError {
    super();
  }

  @Override
  protected void finalize() throws Throwable {
    destroyAllBrowser();
    removeContextMenuHandler(this);
    removeDialogHandler(this);
    removeDisplayHandler(this);
    removeDownloadHandler(this);
    removeFocusHandler(this);
    removeLifeSpanHandler(this);
    removeMessageRouterHandler(this);
    removeLoadHandler(this);
    removeRenderHandler(this);
    removeDragHandler(this);
    super.finalize();
  }

  public CefBrowser createBrowser(String url,
                                  boolean isOffscreenRendered,
                                  boolean isTransparent) {
    return CefBrowserFactory.create(this, url, isOffscreenRendered, isTransparent);
  }

  public void destroyBrowser(CefBrowser browser) {
    Integer browserId = new Integer(browser.getIdentifier());
    if (browser_.remove(browserId) != null) {
      browser.close();
    }
  }

  @Override
  protected CefBrowser getBrowser(int identifier) {
    return browser_.get(new Integer(identifier));
  }

  public void destroyAllBrowser() {
    Collection<CefBrowser> browserList = browser_.values();
    for (CefBrowser browser : browserList) {
      browser.close();
    }
    browser_.clear();
  }

  public CefClient addContextMenuHandler(CefContextMenuHandler handler) {
    if (contextMenuHandler_ == null)
      contextMenuHandler_ = handler;
    return this;
  }

  public void removeContextMenuHandler() {
    contextMenuHandler_ = null;
  }

  public CefClient addDialogHandler(CefDialogHandler handler) {
    if (dialogHandler_ == null)
      dialogHandler_ = handler;
    return this;
  }

  public void removeDialogHandler() {
    dialogHandler_ = null;
  }

  public CefClient addDisplayHandler(CefDisplayHandler handler) {
    if (displayHandler_ == null)
      displayHandler_ = handler;
    return this;
  }

  public void removeDisplayHandler() {
    displayHandler_ = null;
  }

  public CefClient addDownloadHandler(CefDownloadHandler handler) {
    if (downloadHandler_ == null)
      downloadHandler_ = handler;
    return this;
  }

  public void removeDownloadHandler() {
    downloadHandler_ = null;
  }

  public CefClient addDragHandler(CefDragHandler handler) {
    if (dragHandler_ == null)
      dragHandler_ = handler;
    return this;
  }

  public void removeDragHandler() {
    dragHandler_ = null;
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
  protected CefContextMenuHandler getContextMenuHandler() {
    return this;
  }

  @Override 
  protected CefDialogHandler getDialogHandler() {
    return this;
  };

  @Override
  protected CefDisplayHandler getDisplayHandler() {
    return this;
  }

  @Override
  protected CefDownloadHandler getDownloadHandler() {
    return this;
  }

  @Override
  protected CefDragHandler getDragHandler() {
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

  @Override
  public boolean onBeforePopup(CefBrowser browser,
                               String target_url,
                               String target_frame_name) {
    if (lifeSpanHandler_ != null && browser != null)
      return lifeSpanHandler_.onBeforePopup(browser,
                                            target_url,
                                            target_frame_name);
    return false;
  }

  @Override
  public void onAfterCreated(CefBrowser browser) {
    if (browser == null)
      return;

    // keep browser reference
    Integer identifier = browser.getIdentifier();
    browser_.put(identifier, browser);
    if (lifeSpanHandler_ != null)
      lifeSpanHandler_.onAfterCreated(browser);
  }

  @Override
  public boolean runModal(CefBrowser browser) {
    if (lifeSpanHandler_ != null && browser != null)
      return lifeSpanHandler_.runModal(browser);
    return false;
  }

  @Override
  public boolean doClose(CefBrowser browser) {
    if (lifeSpanHandler_ != null && browser != null)
      return lifeSpanHandler_.doClose(browser);
    return false;
  }

  @Override
  public void onBeforeClose(CefBrowser browser) {
    if (lifeSpanHandler_ != null && browser != null)
      lifeSpanHandler_.onBeforeClose(browser);
  }

  @Override
  public void onAddressChange(CefBrowser browser, String url) {
    if (displayHandler_ != null && browser != null)
      displayHandler_.onAddressChange(browser, url);
  }

  @Override
  public void onTitleChange(CefBrowser browser, String title) {
    if (displayHandler_ != null && browser != null)
      displayHandler_.onTitleChange(browser, title);
  }

  @Override
  public boolean onTooltip(CefBrowser browser, String text) {
    if (displayHandler_ != null && browser != null) {
      return displayHandler_.onTooltip(browser, text);
    }
    return false;
  }

  @Override
  public void onStatusMessage(CefBrowser browser, String value) {
    if (displayHandler_ != null && browser != null) {
      displayHandler_.onStatusMessage(browser, value);
    }
  }

  @Override
  public boolean onConsoleMessage(CefBrowser browser,
                                  String message,
                                  String source,
                                  int line) {
    if (displayHandler_ != null && browser != null) {
      return displayHandler_.onConsoleMessage(browser, message, source, line);
    }
    return false;
  }

  @Override
  public Rectangle getViewRect(CefBrowser browser) {
    if (browser == null)
      return new Rectangle(0,0,0,0);

    CefRenderHandler realHandler = browser.getRenderHandler();
    if (realHandler != null)
      return realHandler.getViewRect(browser);
    
    Rectangle tmp = browser.getUIComponent().getBounds();
    if (OS.isMacintosh()) {
      Container parent = browser.getUIComponent().getParent();
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
    if (browser == null)
      return new Point(0,0);

    CefRenderHandler realHandler = browser.getRenderHandler();
    if (realHandler != null)
      return realHandler.getScreenPoint(browser,viewPoint);
    Point screenPoint = browser.getUIComponent().getLocationOnScreen();
    screenPoint.translate(viewPoint.x, viewPoint.y);
    return screenPoint;
  }

  @Override
  public void onPopupShow(CefBrowser browser,
                          boolean show) {
    if (browser == null)
      return;

    CefRenderHandler realHandler = browser.getRenderHandler();
    if (realHandler != null)
      realHandler.onPopupShow(browser, show);
  }

  @Override
  public void onPopupSize(CefBrowser browser,
                          Rectangle size) {
    if (browser == null)
      return;

    CefRenderHandler realHandler = browser.getRenderHandler();
    if (realHandler != null)
      realHandler.onPopupSize(browser, size);
  }

  @Override
  public void onPaint(CefBrowser browser,
                      boolean popup,
                      Rectangle[] dirtyRects,
                      ByteBuffer buffer,
                      int width,
                      int height) {
    if (browser == null)
      return;

    CefRenderHandler realHandler = browser.getRenderHandler();
    if (realHandler != null)
      realHandler.onPaint(browser, popup, dirtyRects, buffer, width, height);
  }

  @Override
  public void onCursorChange(CefBrowser browser,
                             int cursorType) {
    if (browser == null)
      return;

    CefRenderHandler realHandler = browser.getRenderHandler();
    if (realHandler != null)
      realHandler.onCursorChange(browser, cursorType);
  }

  @Override
  public void onTakeFocus(CefBrowser browser, boolean next) {
    if (browser == null)
      return;

    browser.setFocus(false);
    Container parent = browser.getUIComponent().getParent();
    if (parent != null) {
      FocusTraversalPolicy policy = null;
      while (parent != null) {
        policy = parent.getFocusTraversalPolicy();
        if (policy != null)
          break;
        parent = parent.getParent();
      }
      if (policy != null) {
        Component nextComp = next ? policy.getComponentAfter(parent, browser.getUIComponent())
                                  : policy.getComponentBefore(parent, browser.getUIComponent());
        if (nextComp == null) {
          policy.getDefaultComponent(parent).requestFocus();
        } else {
          nextComp.requestFocus();
        }
      }
    }
    if (focusHandler_ != null)
      focusHandler_.onTakeFocus(browser, next);
  }

  @Override
  public boolean onSetFocus(CefBrowser browser, FocusSource source) {
    if (browser == null)
      return false;

    boolean alreadyHandled = false;
    if (focusHandler_ != null)
      alreadyHandled = focusHandler_.onSetFocus(browser, source);
    if (!alreadyHandled)
      browser.getUIComponent().requestFocus();
    return alreadyHandled;
  }

  @Override
  public void onGotFocus(CefBrowser browser) {
    if (browser == null)
      return;

    browser.setFocus(true);
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
    if (msgRouterHandler_ != null && browser != null)
      alreadyHandled = msgRouterHandler_.onQuery(browser, query_id, request, persistent, callback);
    return alreadyHandled;
  }

  @Override
  public void onQueryCanceled(CefBrowser browser,
                              long query_id) {
    if (msgRouterHandler_ != null && browser != null)
      msgRouterHandler_.onQueryCanceled(browser, query_id);
  }

  @Override
  public void onLoadingStateChange(CefBrowser browser,
                                   boolean isLoading,
                                   boolean canGoBack,
                                   boolean canGoForward) {
    if (loadHandler_ != null && browser != null)
      loadHandler_.onLoadingStateChange(browser, isLoading, canGoBack, canGoForward);
  }

  @Override
  public void onLoadStart(CefBrowser browser, int frameIdentifer) {
    if (loadHandler_ != null && browser != null)
      loadHandler_.onLoadStart(browser, frameIdentifer);
  }

  @Override
  public void onLoadEnd(CefBrowser browser,
                        int frameIdentifier,
                        int httpStatusCode) {
    if (loadHandler_ != null && browser != null)
      loadHandler_.onLoadEnd(browser, frameIdentifier, httpStatusCode);
  }

  @Override
  public void onLoadError(CefBrowser browser,
                          int frameIdentifer,
                          ErrorCode errorCode,
                          String errorText,
                          String failedUrl) {
    if (loadHandler_ != null && browser != null)
      loadHandler_.onLoadError(browser, frameIdentifer, errorCode, errorText, failedUrl);
  }

  @Override
  public boolean onFileDialog(CefBrowser browser,
                              FileDialogMode mode,
                              String title,
                              String defaultFileName,
                              Vector<String> acceptTypes,
                              CefFileDialogCallback callback) {
    if (dialogHandler_ != null && browser != null)
      return dialogHandler_.onFileDialog(browser, mode, title, defaultFileName, acceptTypes, callback);
    return false;
  }

  @Override
  public void onBeforeDownload(CefBrowser browser,
                               CefDownloadItem downloadItem,
                               String suggestedName,
                               CefBeforeDownloadCallback callback) {
    if (downloadHandler_ != null && browser != null)
      downloadHandler_.onBeforeDownload(browser, downloadItem, suggestedName, callback);
  }

  @Override
  public void onDownloadUpdated(CefBrowser browser,
                                CefDownloadItem downloadItem,
                                CefDownloadItemCallback callback) {
    if (downloadHandler_ != null && browser != null)
      downloadHandler_.onDownloadUpdated(browser, downloadItem, callback);
  }

  @Override
  public void onBeforeContextMenu(CefBrowser browser,
                                  CefContextMenuParams params,
                                  CefMenuModel model) {
    if (contextMenuHandler_ != null && browser != null)
      contextMenuHandler_.onBeforeContextMenu(browser, params, model);
  }

  @Override
  public boolean onContextMenuCommand(CefBrowser browser,
                                      CefContextMenuParams params,
                                      int commandId,
                                      int eventFlags) {
    if (contextMenuHandler_ != null && browser != null)
      return contextMenuHandler_.onContextMenuCommand(browser, params, commandId, eventFlags);
    return false;
  }

  @Override
  public void onContextMenuDismissed(CefBrowser browser) {
    if (contextMenuHandler_ != null && browser != null)
      contextMenuHandler_.onContextMenuDismissed(browser);
  }

  @Override
  public boolean onDragEnter(CefBrowser browser, CefDragData dragData, int mask) {
    if (dragHandler_ != null && browser != null)
      return dragHandler_.onDragEnter(browser, dragData, mask);
    return false;
  }
}
