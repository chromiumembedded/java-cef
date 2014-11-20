// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.handler;

import org.cef.callback.CefCallback;
import org.cef.callback.CefNative;
import org.cef.misc.IntRef;
import org.cef.misc.StringRef;
import org.cef.network.CefCookie;
import org.cef.network.CefRequest;
import org.cef.network.CefResponse;

/**
 * Implement this interface to handle custom resource requests. The methods of
 * this class will always be called on the IO thread.
 */
public interface CefResourceHandler extends CefNative {

  /**
   * Begin processing the request. To handle the request return true and call
   * CefCallback::Continue() once the response header information is available
   * (CefCallback::Continue() can also be called from inside this method if
   * header information is available immediately). To cancel the request return
   * false.
   */
  boolean processRequest(CefRequest request,
                         CefCallback callback);

  /**
   * Retrieve response header information. If the response length is not known
   * set |response_length| to -1 and ReadResponse() will be called until it
   * returns false. If the response length is known set |response_length|
   * to a positive value and ReadResponse() will be called until it returns
   * false or the specified number of bytes have been read. Use the |response|
   * object to set the mime type, http status code and other optional header
   * values. To redirect the request to a new URL set |redirectUrl| to the new
   * URL.
   */
  void getResponseHeaders(CefResponse response,
                          IntRef response_length,
                          StringRef redirectUrl);

  /**
   * Read response data. If data is available immediately copy up to
   * |bytes_to_read| bytes into |data_out|, set |bytes_read| to the number of
   * bytes copied, and return true. To read the data at a later time set
   * |bytes_read| to 0, return true and call CefCallback::Continue() when the
   * data is available. To indicate response completion return false.
   */
  boolean readResponse(byte[] data_out,
                       int bytes_to_read,
                       IntRef bytes_read,
                       CefCallback callback);

  /**
   * Return true if the specified cookie can be sent with the request or false
   * otherwise. If false is returned for any cookie then no cookies will be sent
   * with the request.
   */
  boolean canGetCookie(CefCookie cookie);

  /**
   * Return true if the specified cookie returned with the response can be set
   * or false otherwise.
   */
  boolean canSetCookie(CefCookie cookie);

  /**
   * Request processing has been canceled.
   */
  void cancel();
}
