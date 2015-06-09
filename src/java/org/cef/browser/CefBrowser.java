// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.browser;

import java.awt.Component;
import java.awt.Point;
import java.util.Vector;

import org.cef.callback.CefRunFileDialogCallback;
import org.cef.callback.CefStringVisitor;
import org.cef.handler.CefDialogHandler.FileDialogMode;
import org.cef.handler.CefRenderHandler;
import org.cef.handler.CefWindowHandler;
import org.cef.network.CefRequest;

/**
 * Interface representing a browser.
 */
public interface CefBrowser {

  /**
   * Get the underlying UI component (e.g. java.awt.Canvas).
   * @return The underlying UI component.
   */
  public Component getUIComponent();

  /**
   * Get an implementation of CefRenderHandler if any.
   * @return An instance of CefRenderHandler or null.
   */
  public CefRenderHandler getRenderHandler();

  /**
   * Get an implementation of CefWindowHandler if any.
   * @return An instance of CefWindowHandler or null.
   */
  public CefWindowHandler getWindowHandler();

  //
  // The following methods are forwarded to CefBrowser.
  //

  /**
   * Tests if the browser can navigate backwards.
   * @return true if the browser can navigate backwards.
   */
  public boolean canGoBack();

  /**
   * Go back.
   */
  public void goBack();

  /**
   * Tests if the browser can navigate forwards.
   * @return true if the browser can navigate forwards.
   */
  public boolean canGoForward();

  /**
   * Go forward.
   */
  public void goForward();

  /**
   * Tests if the browser is currently loading.
   * @return true if the browser is currently loading.
   */
  public boolean isLoading();

  /**
   * Reload the current page.
   */
  public void reload();

  /**
   * Reload the current page ignoring any cached data.
   */
  public void reloadIgnoreCache();

  /**
   * Stop loading the page.
   */
  public void stopLoad();

  /**
   * Returns the unique browser identifier.
   * @return The browser identifier
	 */
  public int getIdentifier();

  /**
   * Tests if the window is a popup window.
   * @return true if the window is a popup window.
   */
  public boolean isPopup();

  /**
   * Tests if a document has been loaded in the browser.
   * @return true if a document has been loaded in the browser.
   */
  public boolean hasDocument();

  //
  // The following methods are forwarded to the mainFrame.
  //

  /**
   * Save this frame's HTML source to a temporary file and open it in the
   * default text viewing application. This method can only be called from the
   * browser process.
   */
  public void viewSource();

  /**
   * Retrieve this frame's HTML source as a string sent to the specified
   * visitor.
   * 
   * @param visitor
   */
  public void getSource(CefStringVisitor visitor);

  /**
   * Retrieve this frame's display text as a string sent to the specified
   * visitor.
   * 
   * @param visitor
   */
  public void getText(CefStringVisitor visitor);

  /**
   * Load the request represented by the request object.
   * 
   * @param request The request object.
   */
  public void loadRequest(CefRequest request);

  /**
   * Load the specified URL in the main frame.
   * @param url The URL to load.
   */
  public void loadURL(String url);

  /**
   * Load the contents of val with the specified dummy url.
   * url should have a standard scheme (for example, http scheme) or
   * behaviors like link clicks and web security restrictions may not
   * behave as expected.
   * 
   * @param val Content to be displayed.
   * @param url dummy url to be used for.
   */
  public void loadString(String val, String url);

  /**
   * Execute a string of JavaScript code in this frame. The url
   * parameter is the URL where the script in question can be found, if any.
   * The renderer may request this URL to show the developer the source of the
   * error. The line parameter is the base line number to use for error
   * reporting.
   * 
   * @param code The code to be executed.
   * @param url The URL where the script in question can be found.
   * @param line The base line number to use for error reporting.
   */
  public void executeJavaScript(String code, String url, int line);

  /**
   * Emits the URL currently loaded in this frame.
   * @return the URL currently loaded in this frame.
   */
  public String getURL();

  // The following methods are forwarded to CefBrowserHost.

  /**
   * Close the browser.
   */
  public void close();

  /**
   * Set or remove keyboard focus to/from the browser window.
   * @param enable set to true to give the focus to the browser
   **/
  public void setFocus(boolean enable);

  /**
   * Set whether the window containing the browser is visible
   * (minimized/unminimized, app hidden/unhidden, etc). Only used on Mac OS X.
   * @param visible
   */
  public void setWindowVisibility(boolean visible);

  /**
   * Get the current zoom level. The default zoom level is 0.0.
   * @return The current zoom level.
   */
  public double getZoomLevel();

  /**
   * Change the zoom level to the specified value. Specify 0.0 to reset the
   * zoom level.
   * 
   * @param zoomLevel The zoom level to be set.
   */
  public void setZoomLevel(double zoomLevel);

  /**
   * Call to run a file chooser dialog. Only a single file chooser dialog may be
   * pending at any given time.The dialog will be initiated asynchronously on
   * the UI thread.
   * 
   * @param mode represents the type of dialog to display.
   * @param title  to be used for the dialog and may be empty to show the
   * default title ("Open" or "Save" depending on the mode).
   * @param defaultFilePath is the path with optional directory and/or file name
   * component that should be initially selected in the dialog.
   * @param acceptFilters are used to restrict the selectable file types and may
   * any combination of (a) valid lower-cased MIME types (e.g. "text/*" or
   * "image/*"), (b) individual file extensions (e.g. ".txt" or ".png"), or (c)
   * combined description and file extension delimited using "|" and ";" (e.g.
   * "Image Types|.png;.gif;.jpg").
   * @param selectedAcceptFilter is the 0-based index of the filter that should
   * be selected by default.
   * @param callback will be executed after the dialog is dismissed or 
   * immediately if another dialog is already pending.
   */
  public void runFileDialog(FileDialogMode mode,
                            String title,
                            String defaultFilePath,
                            Vector<String> acceptFilters,
                            int selectedAcceptFilter,
                            CefRunFileDialogCallback callback);

  /**
   * Download the file at url using CefDownloadHandler.
   * 
   * @param url URL to download that file.
   */
  public void startDownload(String url);

  /**
   * Print the current browser contents.
   */
  public void print();

  /**
   * Search for some kind of text on the page.
   * 
   * @param identifier can be used to have multiple searches running simultaniously.
   * @param searchText to be searched for.
   * @param forward indicates whether to search forward or backward within the page.
   * @param matchCase indicates whether the search should be case-sensitive.
   * @param findNext indicates whether this is the first request or a follow-up.
   */
  public void find(int identifier,
                   String searchText,
                   boolean forward,
                   boolean matchCase,
                   boolean findNext);

  /**
   * Cancel all searches that are currently going on.
   * @param clearSelection Set to true to reset selection.
   */
  public void stopFinding(boolean clearSelection);

  /**
   * Get an instance of the dev tools to be displayed in its own window or to be
   * embedded within your UI. Only one instance per browser is available.
   */
  public CefBrowser getDevTools();

  /**
   * Get an instance of the dev tools to be displayed in its own window or to be
   * embedded within your UI. Only one instance per browser is available.
   * 
   * @param inspectAt a position in the UI which should be inspected.
   */
  public CefBrowser getDevTools(Point inspectAt);

  /**
   * If a misspelled word is currently selected in an editable node calling
   * this method will replace it with the specified |word|.
   *
   * @param word replace selected word with this word.
   */
  public void replaceMisspelling(String word);
}
