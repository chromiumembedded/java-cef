// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.handler;

import org.cef.browser.CefBrowser;
import org.cef.callback.CefAllowCertificateErrorCallback;
import org.cef.callback.CefAuthCallback;
import org.cef.callback.CefQuotaCallback;
import org.cef.network.CefRequest;

/**
 * Implement this interface to handle events related to browser requests. The
 * methods of this class will be called on the thread indicated.
 */
public interface CefRequestHandler {

  /**
   * Called on the UI thread before browser navigation. Return true to cancel
   * the navigation or false to allow the navigation to proceed. The request
   * object cannot be modified in this callback.
   * 
   * CefLoadHandler.onLoadingStateChange() will be called twice in all cases.
   * If the navigation is allowed CefLoadHandler.onLoadStart() and
   * CefLoadHandler.onLoadEnd() will be called. If the navigation is canceled
   * CefLoadHandler.onLoadError() will be called with an errorCode value of
   * ERR_ABORTED.
   * 
   * @param browser The corresponding browser.
   * @param request The request itself. Can't be modified.
   * @param is_redirect true if the request was redirected.
   * @return true to cancel or false to allow to proceed.
   */
  boolean onBeforeBrowse(CefBrowser browser,
                         CefRequest request,
                         boolean is_redirect);

  /**
   * Called on the IO thread before a resource request is loaded.
   * 
   * @param browser The corresponding browser.
   * @param request The request object may be modified.
   * @return To cancel the request return true otherwise return false.
   */
  boolean onBeforeResourceLoad(CefBrowser browser,
                               CefRequest request);

  /**
   * Called on the IO thread before a resource is loaded. To allow the resource
   * to load normally return NULL. To specify a handler for the resource return
   * a CefResourceHandler object. The |request| object should not be modified in
   * this callback.
   * 
   * @param browser The corresponding browser.
   * @param request The request itself. Should not be modified in this callback.
   * @return a CefResourceHandler instance or NULL.
   */
  CefResourceHandler getResourceHandler(CefBrowser browser,
                                        CefRequest request);

  // TODO(jcef) add void onResourceRedirect(...)

  /**
   * Called on the IO thread when the browser needs credentials from the user.
   * Return true to continue the request and call CefAuthCallback::Continue()
   * when the authentication information is available.
   * Return false to cancel the request.
   * 
   * @param browser The corresponding browser.
   * @param isProxy indicates whether the host is a proxy server.
   * @param host contains the hostname.
   * @param port contains the port number.
   * @param realm The realm of the request.
   * @param scheme The scheme of the request.
   * @param callback call CefAuthCallback::Continue() when the authentication
   *   information is available.
   * @return true to continue the request or false to cancel.
   */
  boolean getAuthCredentials(CefBrowser browser,
                             boolean isProxy,
                             String host,
                             int port,
                             String realm,
                             String scheme,
                             CefAuthCallback callback);

  /**
   * Called on the IO thread when JavaScript requests a specific storage quota
   * size via the webkitStorageInfo.requestQuota function.
   * 
   * @param browser The corresponding browser.
   * @param origin_url is the origin of the page making the request.
   * @param new_size is the requested quota size in bytes.
   * @param callback call CefQuotaCallback::Continue() either in this method or
   *   at a later time to grant or deny the request.
   * @return true to handle the request or false to cancel the request.
   */
  boolean onQuotaRequest(CefBrowser browser,
                         String origin_url,
                         long new_size,
                         CefQuotaCallback callback);

  // TODO(jcef) add void onProtocolExecution(...)

  /**
   * Called on the UI thread to handle requests for URLs with an invalid
   * SSL certificate. Return true and call CefAllowCertificateErrorCallback::
   * Continue() either in this method or at a later time to continue or cancel
   * the request. Return false to cancel the request immediately. If callback
   * is empty the error cannot be recovered from and the request will be
   * canceled automatically. If "ignore-certificate-errors" command-line switch
   * is set all invalid certificates will be accepted without calling this method.
   *
   * @param cert_error Error code describing the error.
   * @param request_url The requesting URL.
   * @param callback call CefAllowCertificateErrorCallback::Continue() to continue 
   *   or cancel the request.
   * @return true to handle the request or false to reject it.
   */
  boolean onCertificateError(CefLoadHandler.ErrorCode cert_error,
                             String request_url,
                             CefAllowCertificateErrorCallback callback);

  // TODO(jcef) add boolean onBeforePluginLoad(...)
  // TODO(jcef) add void onPluginCrashed(...)
  // TODO(jcef) add void onRenderProcessTerminated(...)
}
