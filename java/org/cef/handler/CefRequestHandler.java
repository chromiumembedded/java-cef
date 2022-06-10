// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.handler;

import org.cef.browser.CefBrowser;
import org.cef.browser.CefFrame;
import org.cef.callback.CefAuthCallback;
import org.cef.callback.CefCallback;
import org.cef.misc.BoolRef;
import org.cef.network.CefRequest;
import org.cef.network.CefURLRequest;

/**
 * Implement this interface to handle events related to browser requests. The methods of this class
 * will be called on the thread indicated.
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
     * Called on the UI thread before browser navigation.
     *
     * CefLoadHandler.onLoadingStateChange() will be called twice in all cases. If the navigation is
     * allowed CefLoadHandler.onLoadStart() and CefLoadHandler.onLoadEnd() will be called. If the
     * navigation is canceled CefLoadHandler.onLoadError() will be called with an errorCode value of
     * ERR_ABORTED.
     *
     * @param browser The corresponding browser.
     * @param frame The frame generating the event. Instance only valid within the scope of this
     *         method.
     * @param request The request itself. Cannot be modified in this callback. Instance only valid
     *         within the scope of this method.
     * @param user_gesture True if the request was initiated by a user gesture.
     * @param is_redirect True if the request was redirected.
     * @return True to cancel the navigation or false to continue.
     */
    boolean onBeforeBrowse(CefBrowser browser, CefFrame frame, CefRequest request,
            boolean user_gesture, boolean is_redirect);

    /**
     * Called on the UI thread before OnBeforeBrowse in certain limited cases
     * where navigating a new or different browser might be desirable. This
     * includes user-initiated navigation that might open in a special way (e.g.
     * links clicked via middle-click or ctrl + left-click) and certain types of
     * cross-origin navigation initiated from the renderer process (e.g.
     * navigating the top-level frame to/from a file URL).
     *
     * @param browser The corresponding browser.
     * @param frame The frame generating the event. Instance only valid within the scope of this
     *         method
     * @param user_gesture True if the request was initiated by a user gesture.
     * @return True to cancel navigation or false to continue
     */
    boolean onOpenURLFromTab(
            CefBrowser browser, CefFrame frame, String target_url, boolean user_gesture);

    /**
     * Called on the IO thread before a resource request is initiated. The |browser| and |frame|
     * values represent the source of the request. If this callback returns null the same method
     * will be called on the associated CefRequestContextHandler, if any.
     *
     * @param browser The corresponding browser.
     * @param frame The frame generating the event. Instance only valid within the scope of this
     *         method.
     * @param request The request itself. Cannot be modified in this callback. Instance only valid
     *         within the scope of this method.
     * @param isNavigation True if the resource request is a navigation.
     * @param isDownload True if the resource request is a download.
     * @param requestInitiator The origin (scheme + domain) of the page that initiated the request.
     * @param disableDefaultHandling Set to true to disable default handling of the request, in
     *         which case it will need to be handled via
     *         CefResourceRequestHandler.getResourceHandler or it will be canceled.
     * @return A CefResourceRequestHandler instance or null.
     */
    CefResourceRequestHandler getResourceRequestHandler(CefBrowser browser, CefFrame frame,
            CefRequest request, boolean isNavigation, boolean isDownload, String requestInitiator,
            BoolRef disableDefaultHandling);

    /**
     * Called on the IO thread when the browser needs credentials from the user.
     *
     * @param browser The corresponding browser.
     * @param origin_url The origin making this authentication request.
     * @param isProxy True if the host is a proxy server.
     * @param host Hostname.
     * @param port Port number.
     * @param realm Realm of the request.
     * @param scheme Scheme of the request.
     * @param callback Call CefAuthCallback.Continue() when the authentication information is
     *         available.
     * @return True to continue the request (callback must be executed) or false to cancel.
     */
    boolean getAuthCredentials(CefBrowser browser, String origin_url, boolean isProxy, String host,
            int port, String realm, String scheme, CefAuthCallback callback);

    /**
     * Called on the IO thread when JavaScript requests a specific storage quota size via the
     * webkitStorageInfo.requestQuota function.
     *
     * @param browser The corresponding browser.
     * @param origin_url Origin of the page making the request.
     * @param new_size Requested quota size in bytes.
     * @param callback Call CefCallback.Continue() either in this method or at a later time
     *         to grant or deny the request.
     * @return True to handle the request (callback must be executed) or false to cancel.
     */
    boolean onQuotaRequest(
            CefBrowser browser, String origin_url, long new_size, CefCallback callback);

    /**
     * Called on the UI thread to handle requests for URLs with an invalid SSL certificate. If
     * "ignore-certificate-errors" command-line switch is set all invalid certificates will be
     * accepted without calling this method.
     *
     * @param browser The corresponding browser.
     * @param cert_error Error code describing the error.
     * @param request_url The requesting URL.
     * @param callback Call CefCallback.Continue() either in this method or at a later time
     *         to continue or cancel the request. If null the error cannot be recovered from and the
     *         request will be canceled automatically.
     * @return True to handle the request (callback must be executed) or false to reject it.
     */
    boolean onCertificateError(CefBrowser browser, CefLoadHandler.ErrorCode cert_error,
            String request_url, CefCallback callback);

    /**
     * Called on the browser process UI thread when the render process terminates unexpectedly.
     * @param browser The corresponding browser.
     * @param status Indicates how the process was  terminated.
     */
    void onRenderProcessTerminated(CefBrowser browser, TerminationStatus status);
}
