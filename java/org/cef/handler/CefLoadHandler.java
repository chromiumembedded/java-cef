// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.handler;

import java.util.HashMap;
import java.util.Map;
import org.cef.browser.CefBrowser;
import org.cef.browser.CefFrame;
import org.cef.network.CefRequest.TransitionType;

/**
 * Implement this interface to handle events related to browser load status.
 */
public interface CefLoadHandler {
    enum ErrorCode {
        ERR_NONE(0),
        ERR_FAILED(-2),
        ERR_ABORTED(-3),
        ERR_INVALID_ARGUMENT(-4),
        ERR_INVALID_HANDLE(-5),
        ERR_FILE_NOT_FOUND(-6),
        ERR_TIMED_OUT(-7),
        ERR_FILE_TOO_BIG(-8),
        ERR_UNEXPECTED(-9),
        ERR_ACCESS_DENIED(-10),
        ERR_NOT_IMPLEMENTED(-11),
        ERR_CONNECTION_CLOSED(-100),
        ERR_CONNECTION_RESET(-101),
        ERR_CONNECTION_REFUSED(-102),
        ERR_CONNECTION_ABORTED(-103),
        ERR_CONNECTION_FAILED(-104),
        ERR_NAME_NOT_RESOLVED(-105),
        ERR_INTERNET_DISCONNECTED(-106),
        ERR_SSL_PROTOCOL_ERROR(-107),
        ERR_ADDRESS_INVALID(-108),
        ERR_ADDRESS_UNREACHABLE(-109),
        ERR_SSL_CLIENT_AUTH_CERT_NEEDED(-110),
        ERR_TUNNEL_CONNECTION_FAILED(-111),
        ERR_NO_SSL_VERSIONS_ENABLED(-112),
        ERR_SSL_VERSION_OR_CIPHER_MISMATCH(-113),
        ERR_SSL_RENEGOTIATION_REQUESTED(-114),
        ERR_CERT_COMMON_NAME_INVALID(-200),
        ERR_CERT_BEGIN(-200), // same as ERR_CERT_COMMON_NAME_INVALID
        ERR_CERT_DATE_INVALID(-201),
        ERR_CERT_AUTHORITY_INVALID(-202),
        ERR_CERT_CONTAINS_ERRORS(-203),
        ERR_CERT_NO_REVOCATION_MECHANISM(-204),
        ERR_CERT_UNABLE_TO_CHECK_REVOCATION(-205),
        ERR_CERT_REVOKED(-206),
        ERR_CERT_INVALID(-207),
        ERR_CERT_WEAK_SIGNATURE_ALGORITHM(-208),
        // -209 is available: was ERR_CERT_NOT_IN_DNS.
        ERR_CERT_NON_UNIQUE_NAME(-210),
        ERR_CERT_WEAK_KEY(-211),
        ERR_CERT_NAME_CONSTRAINT_VIOLATION(-212),
        ERR_CERT_VALIDITY_TOO_LONG(-213),
        ERR_CERT_END(-213), // same as ERR_CERT_VALIDITY_TOO_LONG
        ERR_INVALID_URL(-300),
        ERR_DISALLOWED_URL_SCHEME(-301),
        ERR_UNKNOWN_URL_SCHEME(-302),
        ERR_TOO_MANY_REDIRECTS(-310),
        ERR_UNSAFE_REDIRECT(-311),
        ERR_UNSAFE_PORT(-312),
        ERR_INVALID_RESPONSE(-320),
        ERR_INVALID_CHUNKED_ENCODING(-321),
        ERR_METHOD_NOT_SUPPORTED(-322),
        ERR_UNEXPECTED_PROXY_AUTH(-323),
        ERR_EMPTY_RESPONSE(-324),
        ERR_RESPONSE_HEADERS_TOO_BIG(-325),
        ERR_CACHE_MISS(-400),
        ERR_INSECURE_RESPONSE(-501);

        static private final Map<Integer, ErrorCode> CODES = new HashMap<>();
        static {
            for (ErrorCode ec : ErrorCode.values()) {
                // only put first value into map (so enums listed first have
                // priority over others for duplicate error code values)
                if (!CODES.containsKey(ec.code)) {
                    CODES.put(ec.code, ec);
                }
            }
        }

        private final int code;

        ErrorCode(int code) {
            this.code = code;
        }

        /**
         * Gets the underlying native chrome embedded framework error code value
         * as an integer.
         * @return The error code as an integer
         */
        public int getCode() {
            return code;
        }

        /**
         * Finds the ErrorCode by the native chrome embedded framework integer-based
         * error code value.
         * @param code The integer-based raw error code
         * @return The Java enum mapped to that error code or null if none was found
         */
        static public ErrorCode findByCode(int code) {
            return CODES.get(code);
        }
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
    public void onLoadingStateChange(
            CefBrowser browser, boolean isLoading, boolean canGoBack, boolean canGoForward);

    /**
     * Called when the browser begins loading a frame. The frameIdentifer value will
     * never be empty. Multiple frames may be loading at the same time. Sub-frames may
     * start or continue loading after the main frame load has ended. This method
     * may not be called for a particular frame if the load request for that frame
     * fails. For notification of overall browser load status use
     * OnLoadingStateChange instead.
     *
     * @param browser The affected browser.
     * @param frame The loading frame.
     * @param transitionType The transition type
     */
    public void onLoadStart(CefBrowser browser, CefFrame frame, TransitionType transitionType);

    /**
     * Called when the browser is done loading a frame. The frameIdentifer value will
     * never be empty. Multiple frames may be loading at the same time. Sub-frames may
     * start or continue loading after the main frame load has ended. This method
     * will always be called for all frames irrespective of whether the request
     * completes successfully.
     *
     * @param browser The affected browser.
     * @param frame The loading frame.
     * @param httpStatusCode The status code of the load.
     */
    public void onLoadEnd(CefBrowser browser, CefFrame frame, int httpStatusCode);

    /**
     * Called when the resource load for a navigation fails or is canceled.
     *
     * @param browser The affected browser.
     * @param frame The loading frame.
     * @param errorCode The error code number.
     * @param errorText The error text.
     * @param failedUrl The URL that failed to load.
     */
    public void onLoadError(CefBrowser browser, CefFrame frame, ErrorCode errorCode,
            String errorText, String failedUrl);
}
