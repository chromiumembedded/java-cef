// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.network;

import java.util.Vector;

import org.cef.callback.CefCompletionCallback;
import org.cef.callback.CefCookieVisitor;

/**
 * Class used for managing cookies. The methods of this class may be called on
 * any thread unless otherwise indicated.
 */
public abstract class CefCookieManager {

  // This CTOR can't be called directly. Call method create() instead.
  CefCookieManager() {
  }

  /**
   * Returns the global cookie manager. By default data will be stored at
   * CefSettings.cache_path if specified or in memory otherwise.
   */
  public static final CefCookieManager getGlobalManager() {
    return CefCookieManager_N.getGlobalManagerNative();
  }

  /**
   * Creates a new cookie manager. If |path| is empty data will be stored in
   * memory only. Otherwise, data will be stored at the specified |path|. To
   * persist session cookies (cookies without an expiry date or validity
   * interval) set |persistSessionCookies| to true. Session cookies are
   * generally intended to be transient and most Web browsers do not persist
   * them.
   *
   * @return null if creation fails.
   */
  public static final CefCookieManager createManager(String path,
                                                     boolean persistSessionCookies) {
    return CefCookieManager_N.createNative(path, persistSessionCookies);
  }

  /**
   * Set the schemes supported by this manager. By default only "http" and
   * "https" schemes are supported. Must be called before any cookies are
   * accessed.
   */
  public abstract void setSupportedSchemes(Vector<String> schemes);

  /**
   * Visit all cookies. The returned cookies are ordered by longest path, then
   * by earliest creation date. Returns false if cookies cannot be accessed.
   */
  public abstract boolean visitAllCookies(CefCookieVisitor visitor);

  /**
   * Visit a subset of cookies. The results are filtered by the given url
   * scheme, host, domain and path. If |includeHttpOnly| is true HTTP-only
   * cookies will also be included in the results. The returned cookies are
   * ordered by longest path, then by earliest creation date. Returns false if
   * cookies cannot be accessed.
   */
  public abstract boolean visitUrlCookies(String url,
                                          boolean includeHttpOnly,
                                          CefCookieVisitor visitor);

  /**
   * Sets a cookie given a valid URL and explicit user-provided cookie
   * attributes. This function expects each attribute to be well-formed. It will
   * check for disallowed characters (e.g. the ';' character is disallowed
   * within the cookie value attribute). This method will be called on the
   * IO thread. If posting the task was successful the method returns true.
   */
  public abstract boolean setCookie(String url, CefCookie cookie);

  /**
   * Delete all cookies that match the specified parameters. If both |url| and
   * values |cookieName| are specified all host and domain cookies matching
   * both will be deleted. If only |url| is specified all host cookies (but not
   * domain cookies) irrespective of path will be deleted. If |url| is empty all
   * cookies for all hosts and domains will be deleted. This method will be 
   * called on the IO thread. If posting the task was successful the method
   * returns true.
   */
  public abstract boolean deleteCookies(String url, String cookieName);

  /**
   *  Sets the directory path that will be used for storing cookie data. If
   * |path| is empty data will be stored in memory only. Otherwise, data will be
   * stored at the specified |path|. To persist session cookies (cookies without
   * an expiry date or validity interval) set |persistSessionCookies| to true.
   * Session cookies are generally intended to be transient and most Web browsers
   * do not persist them. Returns false if cookies cannot be accessed.
   */
  public abstract boolean setStoragePath(String path,
                                         boolean persistSessionCookies);

  /**
   * Flush the backing store (if any) to disk and execute the specified
   * |handler| on the IO thread when done. Returns false if cookies cannot be
   * accessed.
   */
  public abstract boolean flushStore(CefCompletionCallback handler);
}
