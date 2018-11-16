// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.handler;

import org.cef.browser.CefBrowser;
import org.cef.browser.CefFrame;
import org.cef.callback.CefAuthCallback;
import org.cef.callback.CefRequestCallback;
import org.cef.misc.BoolRef;
import org.cef.misc.StringRef;
import org.cef.network.CefRequest;
import org.cef.network.CefResponse;
import org.cef.network.CefURLRequest;

/**
 * Implement this interface to handle events related to browser requests. The
 * methods of this class will be called on the thread indicated.
 */
public interface CefRequestHandler {
    /**
     * Process termination status values.
     */
    enum TerminationStatus {
        TS_ABNORMAL_TERMINATION, //!< Non-zero exit status.
        TS_PROCESS_WAS_KILLED, //!< SIGKILL or task manager kill.
        TS_PROCESS_CRASHED, //!< Segmentation fault.
        TS_PROCESS_OOM //!< Out of memory.
    }

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
     * @param frame The frame generating the event. Instance only valid within
     *      the scope of this method.
     * @param request The request itself. Can't be modified.
     * @param user_gesture true if the request was initiated by a user gesture.
     * @param is_redirect true if the request was redirected.
     * @return true to cancel or false to allow to proceed.
     */
    boolean onBeforeBrowse(CefBrowser browser, CefFrame frame, CefRequest request,
            boolean user_gesture, boolean is_redirect);

    /**
     * Called on the IO thread before a resource request is loaded.
     *
     * @param browser The corresponding browser.
     * @param frame The frame generating the event. Instance only valid within
     *      the scope of this method.
     * @param request The request object may be modified.
     * @param callback The request object may be modified.
     * @return To cancel the request return true otherwise return false.
     */
    boolean onBeforeResourceLoad(CefBrowser browser, CefFrame frame, CefRequest request
            // CefRequestCallback callback
            );

    /**
     * Called on the IO thread before a resource is loaded. To allow the resource
     * to load normally return NULL. To specify a handler for the resource return
     * a CefResourceHandler object. The |request| object should not be modified in
     * this callback.
     *
     * @param browser The corresponding browser.
     * @param frame The frame generating the event. Instance only valid within
     *      the scope of this method.
     * @param request The request itself. Should not be modified in this callback.
     * @return a CefResourceHandler instance or NULL.
     */
    CefResourceHandler getResourceHandler(CefBrowser browser, CefFrame frame, CefRequest request);

    /**
     * Called on the IO thread when a resource load is redirected.
     *
     * @param browser The corresponding browser.
     * @param frame The frame generating the event. Instance only valid within
     *      the scope of this method.
     * @param request The request itself. Should not be modified in this callback.
     * @param response The response that resulted in the redirect. Should not be
     *   modified in this callback.
     * @param new_url Contains the new URL and can be changed if desired.
     */
    void onResourceRedirect(CefBrowser browser, CefFrame frame, CefRequest request,
            CefResponse response, StringRef new_url);

    /**
     * Called on the IO thread when a resource response is received. To allow the
     * resource to load normally return false. To redirect or retry the resource
     * modify |request| (url, headers or post body) and return true. The
     * |response| object cannot be modified in this callback.
     * @param browser The corresponding browser.
     * @param frame The frame generating the event. Instance only valid within
     *      the scope of this method.
     * @param request The request itself. To redirect or retry the resource
     *   modify |request| (url, headers or post body) and return true
     * @param response The response that resulted in the redirect. Should not be
     *   modified in this callback.
     * @return True if request modified or false otherwise
     */
    boolean onResourceResponse(
            CefBrowser browser, CefFrame frame, CefRequest request, CefResponse response);

    /**
     * Called on the IO thread when a resource load has completed. |request| and
     * |response| represent the request and response respectively and cannot be
     * modified in this callback. |status| indicates the load completion status.
     * |received_content_length| is the number of response bytes actually read.
     * @param browser The corresponding browser.
     * @param frame The frame generating the event. Instance only valid within
     *      the scope of this method.
     * @param request The request itself. Should not be modified in this callback.
     * @param response The response that resulted in the redirect. Should not be
     *   modified in this callback.
     * @param status The load completion status
     * @param receivedContentLength The number of bytes read
     */
    void onResourceLoadComplete(CefBrowser browser, CefFrame frame, CefRequest request,
            CefResponse response, CefURLRequest.Status status, long receivedContentLength);

    /**
     * Called on the IO thread when the browser needs credentials from the user.
     * Return true to continue the request and call CefAuthCallback::Continue()
     * when the authentication information is available.
     * Return false to cancel the request.
     *
     * @param browser The corresponding browser.
     * @param frame The frame generating the event. Instance only valid within
     *      the scope of this method.
     * @param isProxy indicates whether the host is a proxy server.
     * @param host contains the hostname.
     * @param port contains the port number.
     * @param realm The realm of the request.
     * @param scheme The scheme of the request.
     * @param callback call CefAuthCallback::Continue() when the authentication
     *   information is available.
     * @return true to continue the request or false to cancel.
     */
    boolean getAuthCredentials(CefBrowser browser, CefFrame frame, boolean isProxy, String host,
            int port, String realm, String scheme, CefAuthCallback callback);

    /**
     * Called on the IO thread when JavaScript requests a specific storage quota
     * size via the webkitStorageInfo.requestQuota function.
     *
     * @param browser The corresponding browser.
     * @param origin_url is the origin of the page making the request.
     * @param new_size is the requested quota size in bytes.
     * @param callback call CefRequestCallback::Continue() either in this method or
     *   at a later time to grant or deny the request.
     * @return true to handle the request or false to cancel the request.
     */
    boolean onQuotaRequest(
            CefBrowser browser, String origin_url, long new_size, CefRequestCallback callback);

    /**
     * Called on the UI thread to handle requests for URLs with an unknown
     * protocol component. Set |allow_os_execution| to true to attempt execution
     * via the registered OS protocol handler, if any.
     * SECURITY WARNING: YOU SHOULD USE THIS METHOD TO ENFORCE RESTRICTIONS BASED
     * ON SCHEME, HOST OR OTHER URL ANALYSIS BEFORE ALLOWING OS EXECUTION.
     */
    void onProtocolExecution(CefBrowser browser, String url, BoolRef allow_os_execution);

    /**
     * Called on the UI thread to handle requests for URLs with an invalid
     * SSL certificate. Return true and call CefAllowCertificateErrorCallback::
     * Continue() either in this method or at a later time to continue or cancel
     * the request. Return false to cancel the request immediately. If callback
     * is empty the error cannot be recovered from and the request will be
     * canceled automatically. If "ignore-certificate-errors" command-line switch
     * is set all invalid certificates will be accepted without calling this method.
     *
     * @param browser The corresponding browser.
     * @param cert_error Error code describing the error.
     * @param request_url The requesting URL.
     * @param callback call CefRequestCallback::Continue() to continue
     * or cancel the request.
     * @return true to handle the request or false to reject it.
     */
    boolean onCertificateError(CefBrowser browser, CefLoadHandler.ErrorCode cert_error,
            String request_url, CefRequestCallback callback);

    /**
     * Called on the browser process UI thread when a plugin has crashed.
     * @param browser  The corresponding browser.
     * @param pluginPath the path of the plugin that crashed.
     */
    void onPluginCrashed(CefBrowser browser, String pluginPath);

    /**
     * Called on the browser process UI thread when the render process
     * terminates unexpectedly. |status| indicates how the process
     * terminated.
     */
    void onRenderProcessTerminated(CefBrowser browser, TerminationStatus status);
}
