// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.callback;

import org.cef.misc.BoolRef;
import org.cef.network.CefCookie;

/**
 * Interface to implement for visiting cookie values. The methods of this class
 * will always be called on the IO thread.
 */
public interface CefCookieVisitor {

  /**
   * Method that will be called once for each cookie. |count| is the 0-based
   * index for the current cookie. |total| is the total number of cookies.
   * Set |deleteCookie| to true to delete the cookie currently being visited.
   * Return false to stop visiting cookies. This method may never be called if
   * no cookies are found.
   */
  public abstract boolean visit(CefCookie cookie,
                                int count,
                                int total,
                                BoolRef delete);
}
