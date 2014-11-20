// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.network;

import org.cef.callback.CefURLRequestClient;
import org.cef.handler.CefLoadHandler.ErrorCode;

/**
 * Class used to make a URL request. URL requests are not associated with a
 * browser instance so no CefClient callbacks will be executed. URL requests
 * can be created on any valid CEF thread in either the browser or render
 * process. Once created the methods of the URL request object must be accessed
 * on the same thread that created it.
 */
public abstract class CefURLRequest {

  public static enum Status {
    UR_UNKNOWN,
    UR_SUCCESS,
    UR_IO_PENDING,
    UR_CANCELED,
    UR_FAILED,
  }

  // This CTOR can't be called directly. Call method create() instead.
  CefURLRequest() {
  }

  /**
   * Create a new URL request. Only GET, POST, HEAD, DELETE and PUT request
   * methods are supported. Multiple post data elements are not supported and
   * elements of type PDE_TYPE_FILE are only supported for requests originating
   * from the browser process. Requests originating from the render process will
   * receive the same handling as requests originating from Web content -- if
   * the response contains Content-Disposition or Mime-Type header values that
   * would not normally be rendered then the response may receive special
   * handling inside the browser (for example, via the file download code path
   * instead of the URL request code path). The |request| object will be marked
   * as read-only after calling this method.
   */
  public static final CefURLRequest create(CefRequest request,
                                           CefURLRequestClient client) {
    return CefURLRequest_N.createNative(request, client);
  }

  public abstract void finalize();

  /**
   * Returns the request object used to create this URL request. The returned
   * object is read-only and should not be modified.
   */
  public abstract CefRequest getRequest();

  /**
   * Returns the client.
   */
  public abstract CefURLRequestClient getClient();

  /**
   * Returns the request status.
   */
  public abstract Status getRequestStatus();

  /**
   * Returns the request error if status is UR_CANCELED or UR_FAILED, or 0
   * otherwise.
   */
  public abstract ErrorCode getRequestError();

  /**
   * Returns the response, or NULL if no response information is available.
   * Response information will only be available after the upload has completed.
   * The returned object is read-only and should not be modified.
   */
  public abstract CefResponse getResponse();

  /**
   * Cancel the request.
   */
  public abstract void cancel();
}
