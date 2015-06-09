// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.handler;

import org.cef.browser.CefBrowser;

/**
 * Implement this interface to handle events related to browser load status.
 */
public interface CefLoadHandler {
  enum ErrorCode {
    ERR_NONE,
    ERR_FAILED,
    ERR_ABORTED,
    ERR_INVALID_ARGUMENT,
    ERR_INVALID_HANDLE,
    ERR_FILE_NOT_FOUND,
    ERR_TIMED_OUT,
    ERR_FILE_TOO_BIG,
    ERR_UNEXPECTED,
    ERR_ACCESS_DENIED,
    ERR_NOT_IMPLEMENTED,
    ERR_CONNECTION_CLOSED,
    ERR_CONNECTION_RESET,
    ERR_CONNECTION_REFUSED,
    ERR_CONNECTION_ABORTED,
    ERR_CONNECTION_FAILED,
    ERR_NAME_NOT_RESOLVED,
    ERR_INTERNET_DISCONNECTED,
    ERR_SSL_PROTOCOL_ERROR,
    ERR_ADDRESS_INVALID,
    ERR_ADDRESS_UNREACHABLE,
    ERR_SSL_CLIENT_AUTH_CERT_NEEDED,
    ERR_TUNNEL_CONNECTION_FAILED,
    ERR_NO_SSL_VERSIONS_ENABLED,
    ERR_SSL_VERSION_OR_CIPHER_MISMATCH,
    ERR_SSL_RENEGOTIATION_REQUESTED,
    ERR_CERT_COMMON_NAME_INVALID,
    ERR_CERT_DATE_INVALID,
    ERR_CERT_AUTHORITY_INVALID,
    ERR_CERT_CONTAINS_ERRORS,
    ERR_CERT_NO_REVOCATION_MECHANISM,
    ERR_CERT_UNABLE_TO_CHECK_REVOCATION,
    ERR_CERT_REVOKED,
    ERR_CERT_INVALID,
    ERR_CERT_END,
    ERR_INVALID_URL,
    ERR_DISALLOWED_URL_SCHEME,
    ERR_UNKNOWN_URL_SCHEME,
    ERR_TOO_MANY_REDIRECTS,
    ERR_UNSAFE_REDIRECT,
    ERR_UNSAFE_PORT,
    ERR_INVALID_RESPONSE,
    ERR_INVALID_CHUNKED_ENCODING,
    ERR_METHOD_NOT_SUPPORTED,
    ERR_UNEXPECTED_PROXY_AUTH,
    ERR_EMPTY_RESPONSE,
    ERR_RESPONSE_HEADERS_TOO_BIG,
    ERR_CACHE_MISS,
    ERR_INSECURE_RESPONSE
  }

  /**
   * Called when the loading state has changed. This callback will be executed
   * twice -- once when loading is initiated either programmatically or by user
   * action, and once when loading is terminated due to completion, cancellation
   * of failure.
   * 
   * @param browser The affected browser.
   * @param isLoading true if it is loading.
   * @param canGoBack true if you can navigate back.
   * @param canGoForward true if you can navigate forward.
   */
  public void onLoadingStateChange(CefBrowser browser,
                                   boolean isLoading,
                                   boolean canGoBack,
                                   boolean canGoForward);

  /**
   * Called when the browser begins loading a frame. The frameIdentifer value will
   * never be empty. Multiple frames may be loading at the same time. Sub-frames may
   * start or continue loading after the main frame load has ended. This method
   * may not be called for a particular frame if the load request for that frame
   * fails. For notification of overall browser load status use
   * OnLoadingStateChange instead.
   * 
   * @param browser The affected browser.
   * @param frameIdentifer The identifier of the loading frame.
   */
  public void onLoadStart(CefBrowser browser, int frameIdentifer);

  /**
   * Called when the browser is done loading a frame. The frameIdentifer value will
   * never be empty. Multiple frames may be loading at the same time. Sub-frames may
   * start or continue loading after the main frame load has ended. This method
   * will always be called for all frames irrespective of whether the request
   * completes successfully.
   * 
   * @param browser The affected browser.
   * @param frameIdentifer The identifier of the loading frame.
   * @oaram httpStatusCode The status code of the load.
   */
  public void onLoadEnd(CefBrowser browser,
                        int frameIdentifier,
                        int httpStatusCode);

  /**
   * Called when the resource load for a navigation fails or is canceled.
   * 
   * @param browser The affected browser.
   * @param frameIdentifer The identifier of the loading frame.
   * @param errorCode The error code number.
   * @param errorText The error text.
   * @param failedUrl The URL that failed to load.
   */
  public void onLoadError(CefBrowser browser,
                          int frameIdentifer,
                          ErrorCode errorCode,
                          String errorText,
                          String failedUrl);
}
