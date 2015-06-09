// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.handler;

import org.cef.browser.CefBrowser;

/**
 * Implement this interface to handle events related to browser life span. The
 * methods of this class will be called on the UI thread unless otherwise
 * indicated.
 */
public interface CefLifeSpanHandler {

  /**
   * Called on the IO thread before a new popup window is created. The |browser|
   * and |frame| parameters represent the source of the popup request. The
   * |target_url| and |target_frame_name| values may be empty if none were
   * specified with the request. The |popupFeatures| structure contains
   * information about the requested popup window. To allow creation of the
   * popup window optionally modify |windowInfo|, |client|, |settings| and
   * |no_javascript_access| and return false. To cancel creation of the popup
   * window return true. The |client| and |settings| values will default to the
   * source browser's values. The |no_javascript_access| value indicates whether
   * the new browser window should be scriptable and in the same process as the
   * source browser.
   * 
   * @param browser The source of the popup request.
   * @param frame The source of the popup request.
   * @param target_url May be empty if none is specified with the request.
   * @param target_frame_name May be empty if none is specified with the request.
   * @param target_disposition Indicates where the user intended to open
   * the popup (e.g. current tab, new tab, etc).
   * @param user_gesture Will be true if the popup was opened via explicit user
   * gesture (e.g. clicking a link) or false if the popup opened automatically
   * (e.g. via the DomContentLoaded event).
   * @param popupFeatures Contains information about the requested popup window
   * @param windowInfo Can be modified.
   * @param client Can be modified.
   * @param settings Can be modified.
   * @param no_javascript_access Can be modified.
   * @return To cancel creation of the popup window return true.
   */
  // TODO(jcef) forward all params form JNI to Java (the commented out ones)
  boolean onBeforePopup(CefBrowser browser,
                        // CefFrame frame,
                        String target_url,
                        String target_frame_name
                        // WindowOpenDisposition target_disposition,
                        // bool user_gesture,
                        // CefPopupFeatures popupFeatures,
                        // CefWindowInfo windowInfo,
                        // CefClient client,
                        //CefBrowserSettings settings,
                        // boolean no_javascript_access
                        );

  /**
   * Handle creation of a new browser window.
   * @param browser The browser generating the event.
   */
  public void onAfterCreated(CefBrowser browser);

  /**
   * Called when a modal window is about to display and the modal loop should
   * begin running. Return false to use the default modal loop implementation or
   * true to use a custom implementation.
   * 
   * @param browser The browser generating the event.
   * @return false to use the default modal loop implementation.
   */
  public boolean runModal(CefBrowser browser);

  /**
   * Called when a browser has received a request to close.
   * 
   * If CEF created an OS window for the browser returning false will send an OS
   * close notification to the browser window's top-level owner (e.g. WM_CLOSE
   * on Windows, performClose: on OS-X and "delete_event" on Linux). If no OS
   * window exists (window rendering disabled) returning false will cause the
   * browser object to be destroyed immediately. Return true if the browser is
   * parented to another window and that other window needs to receive close
   * notification via some non-standard technique.
   * 
   * @param browser The browser generating the event.
   * @return false will send an OS close notification to the browser window's top-level owner.
   */
  public boolean doClose(CefBrowser browser);

  /**
   * Called just before a browser is destroyed.
   * 
   * Release all references to the
   * browser object and do not attempt to execute any methods on the browser
   * object after this callback returns. If this is a modal window and a custom
   * modal loop implementation was provided in runModal() this callback should
   * be used to exit the custom modal loop. See doClose() documentation for
   * additional usage information.
   * 
   * @param browser The browser generating the event.
   */
  void onBeforeClose(CefBrowser browser);
}
