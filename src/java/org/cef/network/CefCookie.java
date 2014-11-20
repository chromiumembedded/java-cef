// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.network;

import java.util.Date;

/**
 * Cookie information.
 */
public final class CefCookie {

  public CefCookie(String name,
                   String value,
                   String domain,
                   String path,
                   boolean secure,
                   boolean httponly,
                   Date creation,
                   Date lastAccess,
                   boolean hasExpires,
                   Date expires) {
    this.name = name;
    this.value = value;
    this.domain = domain;
    this.path = path;
    this.secure = secure;
    this.httponly = httponly;
    this.creation = creation;
    this.lastAccess = lastAccess;
    this.hasExpires = hasExpires;
    this.expires = expires;
  }

  /**
   * The cookie name.
   */
  public final String name;

  /**
   * The cookie value.
   */
  public final String value;

  /**
   * If domain is empty a host cookie will be created instead of a domain
   * cookie. Domain cookies are stored with a leading "." and are visible to
   * sub-domains whereas host cookies are not.
   */
  public final String domain;

  /**
   * If path is non-empty only URLs at or below the path will get the cookie
   * value.
   */
  public final String path;

  /**
   * If secure is true the cookie will only be sent for HTTPS requests.
   */
  public final boolean secure;

  /**
   * If httponly is true the cookie will only be sent for HTTP requests.
   */
  public final boolean httponly;

  /**
   * The cookie creation date. This is automatically populated by the system on
   * cookie creation.
   */
  public final Date creation;

  /**
   * The cookie last access date. This is automatically populated by the system
   * on access.
   */
  public final Date lastAccess;

  /**
   * The cookie expiration date is only valid if |has_expires| is true.
   */
  public final boolean hasExpires;
  public final Date expires;
}
