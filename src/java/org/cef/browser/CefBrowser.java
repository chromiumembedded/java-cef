// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.browser;

import java.awt.Component;

import org.cef.handler.CefClientHandler;
import org.cef.handler.CefNative;
import org.cef.handler.CefRenderHandler;

/**
 * Interface representing a browser.
 */
public interface CefBrowser extends CefNative {

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
   * Open developer tools in its own window.
   */
  void showDevTools(CefClientHandler client);

  /**
   * Explicitly close the developer tools window
   * if one exists for this browser instance.
   */
  void closeDevTools();
}
