// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.browser;

import java.awt.event.KeyEvent;
import java.awt.event.MouseEvent;
import java.awt.event.MouseWheelEvent;
import java.awt.Canvas;
import java.awt.Component;
import java.awt.Point;
import java.awt.Rectangle;
import java.awt.Window;
import java.awt.event.WindowEvent;
import java.util.Vector;

import javax.swing.SwingUtilities;

import org.cef.CefClient;
import org.cef.browser.CefRequestContext;
import org.cef.callback.CefDragData;
import org.cef.callback.CefNativeAdapter;
import org.cef.callback.CefPdfPrintCallback;
import org.cef.callback.CefRunFileDialogCallback;
import org.cef.callback.CefStringVisitor;
import org.cef.handler.CefClientHandler;
import org.cef.handler.CefRenderHandler;
import org.cef.handler.CefDialogHandler.FileDialogMode;
import org.cef.handler.CefWindowHandler;
import org.cef.misc.CefPdfPrintSettings;
import org.cef.network.CefRequest;

/**
 * This class represents all methods which are connected to the
 * native counterpart CEF.
 * The visibility of this class is "package". To create a new
 * CefBrowser instance, please use CefBrowserFactory.
 */
abstract class CefBrowser_N extends CefNativeAdapter implements CefBrowser {
    private boolean isPending_ = false;
    private CefClient client_;
    private String url_;
    private CefRequestContext request_context_;
    private CefBrowser_N parent_ = null;
    private Point inspectAt_ = null;
    private CefBrowser_N devTools_ = null;
    private boolean closeAllowed_ = false;
    private boolean isClosed_ = false;

    protected CefBrowser_N(CefClient client, String url, CefRequestContext context,
            CefBrowser_N parent, Point inspectAt) {
        client_ = client;
        url_ = url;
        request_context_ = context;
        parent_ = parent;
        inspectAt_ = inspectAt;
    }

    protected String getUrl() {
        return url_;
    }

    protected CefRequestContext getRequestContext() {
        return request_context_;
    }

    protected CefBrowser_N getParentBrowser() {
        return parent_;
    }

    protected Point getInspectAt() {
        return inspectAt_;
    }

    protected boolean isClosed() {
        return isClosed_;
    }

    @Override
    public CefClient getClient() {
        return client_;
    }

    @Override
    public CefRenderHandler getRenderHandler() {
        return null;
    }

    @Override
    public CefWindowHandler getWindowHandler() {
        return null;
    }

    @Override
    public synchronized void setCloseAllowed() {
        closeAllowed_ = true;
    }

    @Override
    public synchronized boolean doClose() {
        if (closeAllowed_) {
            // Allow the close to proceed.
            return false;
        }

        SwingUtilities.invokeLater(new Runnable() {
            @Override
            public void run() {
                // Trigger close of the parent window.
                Component parent = SwingUtilities.getRoot(getUIComponent());
                if (parent != null) {
                    parent.dispatchEvent(
                            new WindowEvent((Window) parent, WindowEvent.WINDOW_CLOSING));
                }
            }
        });

        // Cancel the close.
        return true;
    }

    @Override
    public synchronized void onBeforeClose() {
        isClosed_ = true;
        if (request_context_ != null) request_context_.dispose();
        if (parent_ != null) {
            parent_.closeDevTools();
            parent_.devTools_ = null;
            parent_ = null;
        }
    }

    @Override
    public CefBrowser getDevTools() {
        return getDevTools(null);
    }

    @Override
    public synchronized CefBrowser getDevTools(Point inspectAt) {
        if (devTools_ == null) {
            devTools_ = createDevToolsBrowser(client_, url_, request_context_, this, inspectAt);
        }
        return devTools_;
    }

    protected abstract CefBrowser_N createDevToolsBrowser(CefClient client, String url,
            CefRequestContext context, CefBrowser_N parent, Point inspectAt);

    /**
     * Create a new browser.
     */
    protected void createBrowser(CefClientHandler clientHandler, long windowHandle, String url,
            boolean osr, boolean transparent, Component canvas, CefRequestContext context) {
        if (getNativeRef("CefBrowser") == 0 && !isPending_) {
            try {
                isPending_ = N_CreateBrowser(
                        clientHandler, windowHandle, url, osr, transparent, canvas, context);
            } catch (UnsatisfiedLinkError err) {
                err.printStackTrace();
            }
        }
    }

    /**
     * Create a new browser as dev tools
     */
    protected final void createDevTools(CefBrowser_N parent, CefClientHandler clientHandler,
            long windowHandle, boolean osr, boolean transparent, Component canvas,
            Point inspectAt) {
        if (getNativeRef("CefBrowser") == 0 && !isPending_) {
            try {
                isPending_ = N_CreateDevTools(
                        parent, clientHandler, windowHandle, osr, transparent, canvas, inspectAt);
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
        close(true);
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
    public CefFrame getMainFrame() {
        try {
            return N_GetMainFrame();
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
            return null;
        }
    }

    @Override
    public CefFrame getFocusedFrame() {
        try {
            return N_GetFocusedFrame();
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
            return null;
        }
    }

    @Override
    public CefFrame getFrame(long identifier) {
        try {
            return N_GetFrame(identifier);
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
            return null;
        }
    }

    @Override
    public CefFrame getFrame(String name) {
        try {
            return N_GetFrame2(name);
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
            return null;
        }
    }

    @Override
    public Vector<Long> getFrameIdentifiers() {
        try {
            return N_GetFrameIdentifiers();
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
            return null;
        }
    }

    @Override
    public Vector<String> getFrameNames() {
        try {
            return N_GetFrameNames();
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
            return null;
        }
    }

    @Override
    public int getFrameCount() {
        try {
            return N_GetFrameCount();
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
    public void close(boolean force) {
        try {
            N_Close(force);
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
    public void setWindowVisibility(boolean visible) {
        try {
            N_SetWindowVisibility(visible);
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
    public void runFileDialog(FileDialogMode mode, String title, String defaultFilePath,
            Vector<String> acceptFilters, int selectedAcceptFilter,
            CefRunFileDialogCallback callback) {
        try {
            N_RunFileDialog(
                    mode, title, defaultFilePath, acceptFilters, selectedAcceptFilter, callback);
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
    public void printToPDF(
            String path, CefPdfPrintSettings settings, CefPdfPrintCallback callback) {
        if (path == null || path.isEmpty()) {
            throw new IllegalArgumentException("path was null or empty");
        }
        try {
            N_PrintToPDF(path, settings, callback);
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
    }

    @Override
    public void find(int identifier, String searchText, boolean forward, boolean matchCase,
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

    @Override
    public void replaceMisspelling(String word) {
        try {
            N_ReplaceMisspelling(word);
        } catch (UnsatisfiedLinkError ule) {
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
     * Invalidate the UI.
     */
    protected final void invalidate() {
        try {
            N_Invalidate();
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

    /**
     * Call this method when the user drags the mouse into the web view (before
     * calling DragTargetDragOver/DragTargetLeave/DragTargetDrop).
     * |drag_data| should not contain file contents as this type of data is not
     * allowed to be dragged into the web view. File contents can be removed using
     * CefDragData::ResetFileContents (for example, if |drag_data| comes from
     * CefRenderHandler::StartDragging).
     * This method is only used when window rendering is disabled.
     */
    protected final void dragTargetDragEnter(
            CefDragData dragData, Point pos, int modifiers, int allowedOps) {
        try {
            N_DragTargetDragEnter(dragData, pos, modifiers, allowedOps);
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
    }

    /**
     * Call this method each time the mouse is moved across the web view during
     * a drag operation (after calling DragTargetDragEnter and before calling
     * DragTargetDragLeave/DragTargetDrop).
     * This method is only used when window rendering is disabled.
     */
    protected final void dragTargetDragOver(Point pos, int modifiers, int allowedOps) {
        try {
            N_DragTargetDragOver(pos, modifiers, allowedOps);
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
    }

    /**
     * Call this method when the user drags the mouse out of the web view (after
     * calling DragTargetDragEnter).
     * This method is only used when window rendering is disabled.
     */
    protected final void dragTargetDragLeave() {
        try {
            N_DragTargetDragLeave();
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
    }

    /**
     * Call this method when the user completes the drag operation by dropping
     * the object onto the web view (after calling DragTargetDragEnter).
     * The object being dropped is |drag_data|, given as an argument to
     * the previous DragTargetDragEnter call.
     * This method is only used when window rendering is disabled.
     */
    protected final void dragTargetDrop(Point pos, int modifiers) {
        try {
            N_DragTargetDrop(pos, modifiers);
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
    }

    /**
     * Call this method when the drag operation started by a
     * CefRenderHandler.startDragging call has ended either in a drop or
     * by being cancelled. |x| and |y| are mouse coordinates relative to the
     * upper-left corner of the view. If the web view is both the drag source
     * and the drag target then all DragTarget* methods should be called before
     * DragSource* methods.
     * This method is only used when window rendering is disabled.
     */
    protected final void dragSourceEndedAt(Point pos, int operation) {
        try {
            N_DragSourceEndedAt(pos, operation);
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
    }

    /**
     * Call this method when the drag operation started by a
     * CefRenderHandler.startDragging call has completed. This method may be
     * called immediately without first calling DragSourceEndedAt to cancel a
     * drag operation. If the web view is both the drag source and the drag
     * target then all DragTarget* methods should be called before DragSource*
     * methods.
     * This method is only used when window rendering is disabled.
     */
    protected final void dragSourceSystemDragEnded() {
        try {
            N_DragSourceSystemDragEnded();
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
    }

    protected final void updateUI(Rectangle contentRect, Rectangle browserRect) {
        try {
            N_UpdateUI(contentRect, browserRect);
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
    }

    protected final void setParent(long windowHandle, Component canvas) {
        try {
            N_SetParent(windowHandle, canvas);
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
    }

    private final native boolean N_CreateBrowser(CefClientHandler clientHandler, long windowHandle,
            String url, boolean osr, boolean transparent, Component canvas,
            CefRequestContext context);
    private final native boolean N_CreateDevTools(CefBrowser parent, CefClientHandler clientHandler,
            long windowHandle, boolean osr, boolean transparent, Component canvas, Point inspectAt);
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
    private final native CefFrame N_GetMainFrame();
    private final native CefFrame N_GetFocusedFrame();
    private final native CefFrame N_GetFrame(long identifier);
    private final native CefFrame N_GetFrame2(String name);
    private final native Vector<Long> N_GetFrameIdentifiers();
    private final native Vector<String> N_GetFrameNames();
    private final native int N_GetFrameCount();
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
    private final native void N_Close(boolean force);
    private final native void N_SetFocus(boolean enable);
    private final native void N_SetWindowVisibility(boolean visible);
    private final native double N_GetZoomLevel();
    private final native void N_SetZoomLevel(double zoomLevel);
    private final native void N_RunFileDialog(FileDialogMode mode, String title,
            String defaultFilePath, Vector<String> acceptFilters, int selectedAcceptFilter,
            CefRunFileDialogCallback callback);
    private final native void N_StartDownload(String url);
    private final native void N_Print();
    private final native void N_PrintToPDF(
            String path, CefPdfPrintSettings settings, CefPdfPrintCallback callback);
    private final native void N_Find(int identifier, String searchText, boolean forward,
            boolean matchCase, boolean findNext);
    private final native void N_StopFinding(boolean clearSelection);
    private final native void N_CloseDevTools();
    private final native void N_ReplaceMisspelling(String word);
    private final native void N_WasResized(int width, int height);
    private final native void N_Invalidate();
    private final native void N_SendKeyEvent(KeyEvent e);
    private final native void N_SendMouseEvent(MouseEvent e);
    private final native void N_SendMouseWheelEvent(MouseWheelEvent e);
    private final native void N_DragTargetDragEnter(
            CefDragData dragData, Point pos, int modifiers, int allowed_ops);
    private final native void N_DragTargetDragOver(Point pos, int modifiers, int allowed_ops);
    private final native void N_DragTargetDragLeave();
    private final native void N_DragTargetDrop(Point pos, int modifiers);
    private final native void N_DragSourceEndedAt(Point pos, int operation);
    private final native void N_DragSourceSystemDragEnded();
    private final native void N_UpdateUI(Rectangle contentRect, Rectangle browserRect);
    private final native void N_SetParent(long windowHandle, Component canvas);
}
