// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.handler;

import java.util.HashMap;

import org.cef.browser.CefBrowser;

/**
 * Implement this interface to provide handler implementations.
 */
public abstract class CefClientHandler implements CefNative {
  // Used internally to store a pointer to the CEF object.
  private HashMap<String,Long> N_CefHandle = new HashMap<String, Long>();

  @Override
  public void setNativeRef(String identifer, long nativeRef) {
    N_CefHandle.put(identifer, nativeRef);
  }

  @Override
  public long getNativeRef(String identifer) {
    if (N_CefHandle.containsKey(identifer))
      return N_CefHandle.get(identifer);
    return 0;
  }

  public CefClientHandler() {
    try {
      N_CefClientHandler_CTOR();
    } catch (UnsatisfiedLinkError err) {
      err.printStackTrace();
    }
  }

  @Override
  protected void finalize() throws Throwable {
    try {
      // Call native DTOR if handler will be destroyed
      N_CefClientHandler_DTOR();
    } catch (UnsatisfiedLinkError err) {
      err.printStackTrace();
    } finally {
      super.finalize();
    }
  }

  /**
   * Returns the java part of the browser implementation.
   * @param identifer the unique identifier of the browser.
   * @return The found browser or null if none is found.
   */
  abstract protected CefBrowser getBrowser(int identifier);

  /**
   * Return the handler for context menus. If no handler is provided the
   * default implementation will be used.
   */
  abstract protected CefContextMenuHandler getContextMenuHandler();

  /**
   * Return the handler for dialogs. If no handler is provided the
   * default implementation will be used.
   */
  abstract protected CefDialogHandler getDialogHandler();

  /**
   * Return the handler for browser display state events.
   * This method is a callback method and is called by
   * the native code.
   */
  abstract protected CefDisplayHandler getDisplayHandler();

  /**
   * Return the handler for download events.
   * This method is a callback method and is called by
   * the native code.
   */
  abstract protected CefDownloadHandler getDownloadHandler();

  /**
   * Return the handler for drag events.
   * This method is a callback method and is called by
   * the native code.
   */
  abstract protected CefDragHandler getDragHandler();

  /**
   * Return the handler for focus events.
   * This method is a callback method and is called by
   * the native code.
   */
  abstract protected CefFocusHandler getFocusHandler();

  /**
   * Return the handler for browser life span events.
   * This method is a callback method and is called by
   * the native code.
   */
  abstract protected CefLifeSpanHandler getLifeSpanHandler();

  /**
   * Return the handler for message router events.
   * This method is a callback method and is called by
   * the native code.
   */
  abstract protected CefMessageRouterHandler getMessageRouterHandler();

  /**
   * Return the handler for browser load status events.
   * This method is a callback method and is called by
   * the native code.
   */
  abstract protected CefLoadHandler getLoadHandler();

  /**
   * Return the handler for off-screen rendering events.
   * This method is a callback method and is called by
   * the native code.
   */
  abstract protected CefRenderHandler getRenderHandler();

  protected void removeContextMenuHandler(CefContextMenuHandler h) {
    try {
      N_removeContextMenuHandler(h);
    } catch (UnsatisfiedLinkError err) {
      err.printStackTrace();
    }
  }


  protected void removeDialogHandler(CefDialogHandler h) {
    try {
      N_removeDialogHandler(h);
    } catch (UnsatisfiedLinkError err) {
      err.printStackTrace();
    }
  }

  protected void removeDisplayHandler(CefDisplayHandler h) {
    try {
      N_removeDisplayHandler(h);
    } catch (UnsatisfiedLinkError err) {
      err.printStackTrace();
    }
  }

  protected void removeDownloadHandler(CefDisplayHandler h) {
    try {
      N_removeDownloadHandler(h);
    } catch (UnsatisfiedLinkError err) {
      err.printStackTrace();
    }
  }

  protected void removeDragHandler(CefDragHandler h) {
    try {
      N_removeDragHandler(h);
    } catch (UnsatisfiedLinkError err) {
      err.printStackTrace();
    }
  }

  protected void removeFocusHandler(CefFocusHandler h) {
    try {
      N_removeFocusHandler(h);
    } catch (UnsatisfiedLinkError err) {
      err.printStackTrace();
    }
  }

  protected void removeLifeSpanHandler(CefLifeSpanHandler h) {
    try {
      N_removeLifeSpanHandler(h);
    } catch (UnsatisfiedLinkError err) {
      err.printStackTrace();
    }
  }

  protected void removeMessageRouterHandler(CefMessageRouterHandler h) {
    try {
      N_removeMessageRouterHandler(h);
    } catch (UnsatisfiedLinkError err) {
      err.printStackTrace();
    }
  }

  protected void removeLoadHandler(CefLoadHandler h) {
    try {
      N_removeLoadHandler(h);
    } catch (UnsatisfiedLinkError err) {
      err.printStackTrace();
    }
  }

  protected void removeRenderHandler(CefRenderHandler h) {
    try {
      N_removeRenderHandler(h);
    } catch (UnsatisfiedLinkError err) {
      err.printStackTrace();
    }
  }

  private final native void N_CefClientHandler_CTOR();
  private final native void N_removeContextMenuHandler(CefContextMenuHandler h);
  private final native void N_removeDialogHandler(CefDialogHandler h);
  private final native void N_removeDisplayHandler(CefDisplayHandler h);
  private final native void N_removeDownloadHandler(CefDisplayHandler h);
  private final native void N_removeDragHandler(CefDragHandler h);
  private final native void N_removeFocusHandler(CefFocusHandler h);
  private final native void N_removeLifeSpanHandler(CefLifeSpanHandler h);
  private final native void N_removeMessageRouterHandler(CefMessageRouterHandler h);
  private final native void N_removeLoadHandler(CefLoadHandler h);
  private final native void N_removeRenderHandler(CefRenderHandler h);
  private final native void N_CefClientHandler_DTOR();
}
