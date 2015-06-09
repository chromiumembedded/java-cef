// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.network;

import java.util.HashMap;
import java.util.Map;
import java.util.Set;
import java.util.Map.Entry;

/**
 * Class used to represent a web response. The methods of this class may be
 * called on any thread.
 */
public abstract class CefResponse {

  // This CTOR can't be called directly. Call method create() instead.
  CefResponse() {
  }

  /**
   * Create a new CefRequest object.
   */
  public static final CefResponse create() {
    return CefResponse_N.createNative();
  }

  /**
   * Returns true if this object is read-only.
   */
  public abstract boolean isReadOnly();

  /**
   * Get the response status code.
   */
  public abstract int getStatus();

  /**
   * Set the response status code.
   */
  public abstract void setStatus(int status);

  /**
   * Get the response status text.
   */
  public abstract String getStatusText();

  /**
   * Set the response status text.
   */
  public abstract void setStatusText(String statusText);

  /**
   * Get the response mime type.
   */
  public abstract String getMimeType();

  /**
   * Set the response mime type.
   */
  public abstract void setMimeType(String mimeType);

  /**
   * Get the value for the specified response header field.
   */
  public abstract String getHeader(String name);

  /**
   * Get all response header fields.
   */
  public abstract void getHeaderMap(Map<String,String> headerMap);

  /**
   * Set all response header fields.
   */
  public abstract void setHeaderMap(Map<String,String> headerMap);

  @Override
  public String toString() {
    String returnValue = "\nHTTP-Response:";

    returnValue += "\n    HTTP/1.1 " + getStatus() + " " + getStatusText();
    returnValue += "\n    Content-Type: " + getMimeType();

    Map<String, String> headerMap = new HashMap<>();
    getHeaderMap(headerMap);
    Set<Entry<String,String>> entrySet = headerMap.entrySet();
    for (Entry<String,String> entry : entrySet) {
      returnValue += "    " + entry.getKey() + "=" + entry.getValue() + "\n";
    }

    return returnValue;
  }
}
