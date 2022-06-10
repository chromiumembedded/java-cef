// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.network;

import org.cef.callback.CefCompletionCallback;
import org.cef.callback.CefCookieVisitor;

import java.util.Vector;

/**
 * Class used for managing cookies. The methods of this class may be called on any thread unless
 * otherwise indicated.
 */
public abstract class CefCookieManager {
    // This CTOR can't be called directly. Call method create() instead.
    CefCookieManager() {}

    @Override
    protected void finalize() throws Throwable {
        dispose();
        super.finalize();
    }

    /**
     * Returns the global cookie manager. By default data will be stored at CefSettings.cache_path
     * if specified or in memory otherwise.
     * @return The global cookie manager.
     */
    public static final CefCookieManager getGlobalManager() {
        return CefCookieManager_N.getGlobalManagerNative();
    }

    /**
     * Removes the native reference from an unused object.
     */
    public abstract void dispose();

    /**
     * Visit all cookies. The returned cookies are ordered by longest path, then by earliest
     * creation date.
     * @param visitor Callback that will receive cookies on the UI thread.
     * @return False if cookies cannot be accessed.
     */
    public abstract boolean visitAllCookies(CefCookieVisitor visitor);

    /**
     * Visit a subset of cookies. The returned cookies are ordered by longest path, then by earliest
     * creation date.
     * @param url Results are filtered by the given url scheme, host, domain and path.
     * @param includeHttpOnly If true HTTP-only cookies will also be included in the results.
     * @param visitor Callback that will receive cookies on the UI thread.
     * @return False if cookies cannot be accessed.
     */
    public abstract boolean visitUrlCookies(
            String url, boolean includeHttpOnly, CefCookieVisitor visitor);

    /**
     * Sets a cookie given a valid URL and explicit user-provided cookie attributes. This function
     * expects each attribute to be well-formed. It will check for disallowed characters (e.g. the
     * ';' character is disallowed within the cookie value attribute) and fail without setting the
     * cookie if such characters are found.
     * @param url The cookie URL.
     * @param cookie The cookie attributes.
     * @return False if an invalid URL is specified or if cookies cannot be accessed.
     */
    public abstract boolean setCookie(String url, CefCookie cookie);

    /**
     * Delete all cookies that match the specified parameters. If both |url| and |cookieName| values
     * are specified all host and domain cookies matching both will be deleted. If only |url| is
     * specified all host cookies (but not domain cookies) irrespective of path will be deleted. If
     * |url| is empty all cookies for all hosts and domains will be deleted. Cookies can alternately
     * be deleted using the visit*Cookies() methods.
     * @param url The cookie URL to delete or null.
     * @param cookieName The cookie name to delete or null.
     * @return False if a non-empty invalid URL is secified or if cookies cannot be accessed.
     */
    public abstract boolean deleteCookies(String url, String cookieName);

    /**
     * Flush the backing store (if any) to disk.
     * @param handler Callback that will be executed on the UI thread upon completion.
     * @return False if cookies cannot be accessed.
     */
    public abstract boolean flushStore(CefCompletionCallback handler);
}
