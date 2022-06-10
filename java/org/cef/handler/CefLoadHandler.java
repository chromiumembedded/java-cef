// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.handler;

import org.cef.browser.CefBrowser;
import org.cef.browser.CefFrame;
import org.cef.network.CefRequest.TransitionType;

import java.util.HashMap;
import java.util.Map;

/**
 * Implement this interface to handle events related to browser load status.
 */
public interface CefLoadHandler {
    enum ErrorCode {
        ERR_NONE(0),
        ERR_IO_PENDING(-1),
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
        ERR_INSUFFICIENT_RESOURCES(-12),
        ERR_OUT_OF_MEMORY(-13),
        ERR_UPLOAD_FILE_CHANGED(-14),
        ERR_SOCKET_NOT_CONNECTED(-15),
        ERR_FILE_EXISTS(-16),
        ERR_FILE_PATH_TOO_LONG(-17),
        ERR_FILE_NO_SPACE(-18),
        ERR_FILE_VIRUS_INFECTED(-19),
        ERR_BLOCKED_BY_CLIENT(-20),
        ERR_NETWORK_CHANGED(-21),
        ERR_BLOCKED_BY_ADMINISTRATOR(-22),
        ERR_SOCKET_IS_CONNECTED(-23),
        ERR_BLOCKED_ENROLLMENT_CHECK_PENDING(-24),
        ERR_UPLOAD_STREAM_REWIND_NOT_SUPPORTED(-25),
        ERR_CONTEXT_SHUT_DOWN(-26),
        ERR_BLOCKED_BY_RESPONSE(-27),
        ERR_CLEARTEXT_NOT_PERMITTED(-29),
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
        ERR_PROXY_AUTH_UNSUPPORTED(-115),
        ERR_BAD_SSL_CLIENT_AUTH_CERT(-117),
        ERR_CONNECTION_TIMED_OUT(-118),
        ERR_HOST_RESOLVER_QUEUE_TOO_LARGE(-119),
        ERR_SOCKS_CONNECTION_FAILED(-120),
        ERR_SOCKS_CONNECTION_HOST_UNREACHABLE(-121),
        ERR_ALPN_NEGOTIATION_FAILED(-122),
        ERR_SSL_NO_RENEGOTIATION(-123),
        ERR_WINSOCK_UNEXPECTED_WRITTEN_BYTES(-124),
        ERR_SSL_DECOMPRESSION_FAILURE_ALERT(-125),
        ERR_SSL_BAD_RECORD_MAC_ALERT(-126),
        ERR_PROXY_AUTH_REQUESTED(-127),
        ERR_PROXY_CONNECTION_FAILED(-130),
        ERR_MANDATORY_PROXY_CONFIGURATION_FAILED(-131),
        ERR_PRECONNECT_MAX_SOCKET_LIMIT(-133),
        ERR_SSL_CLIENT_AUTH_PRIVATE_KEY_ACCESS_DENIED(-134),
        ERR_SSL_CLIENT_AUTH_CERT_NO_PRIVATE_KEY(-135),
        ERR_PROXY_CERTIFICATE_INVALID(-136),
        ERR_NAME_RESOLUTION_FAILED(-137),
        ERR_NETWORK_ACCESS_DENIED(-138),
        ERR_TEMPORARILY_THROTTLED(-139),
        ERR_HTTPS_PROXY_TUNNEL_RESPONSE_REDIRECT(-140),
        ERR_SSL_CLIENT_AUTH_SIGNATURE_FAILED(-141),
        ERR_MSG_TOO_BIG(-142),
        ERR_WS_PROTOCOL_ERROR(-145),
        ERR_ADDRESS_IN_USE(-147),
        ERR_SSL_HANDSHAKE_NOT_COMPLETED(-148),
        ERR_SSL_BAD_PEER_PUBLIC_KEY(-149),
        ERR_SSL_PINNED_KEY_NOT_IN_CERT_CHAIN(-150),
        ERR_CLIENT_AUTH_CERT_TYPE_UNSUPPORTED(-151),
        ERR_SSL_DECRYPT_ERROR_ALERT(-153),
        ERR_WS_THROTTLE_QUEUE_TOO_LARGE(-154),
        ERR_SSL_SERVER_CERT_CHANGED(-156),
        ERR_SSL_UNRECOGNIZED_NAME_ALERT(-159),
        ERR_SOCKET_SET_RECEIVE_BUFFER_SIZE_ERROR(-160),
        ERR_SOCKET_SET_SEND_BUFFER_SIZE_ERROR(-161),
        ERR_SOCKET_RECEIVE_BUFFER_SIZE_UNCHANGEABLE(-162),
        ERR_SOCKET_SEND_BUFFER_SIZE_UNCHANGEABLE(-163),
        ERR_SSL_CLIENT_AUTH_CERT_BAD_FORMAT(-164),
        ERR_ICANN_NAME_COLLISION(-166),
        ERR_SSL_SERVER_CERT_BAD_FORMAT(-167),
        ERR_CT_STH_PARSING_FAILED(-168),
        ERR_CT_STH_INCOMPLETE(-169),
        ERR_UNABLE_TO_REUSE_CONNECTION_FOR_PROXY_AUTH(-170),
        ERR_CT_CONSISTENCY_PROOF_PARSING_FAILED(-171),
        ERR_SSL_OBSOLETE_CIPHER(-172),
        ERR_WS_UPGRADE(-173),
        ERR_READ_IF_READY_NOT_IMPLEMENTED(-174),
        ERR_NO_BUFFER_SPACE(-176),
        ERR_SSL_CLIENT_AUTH_NO_COMMON_ALGORITHMS(-177),
        ERR_EARLY_DATA_REJECTED(-178),
        ERR_WRONG_VERSION_ON_EARLY_DATA(-179),
        ERR_TLS13_DOWNGRADE_DETECTED(-180),
        ERR_SSL_KEY_USAGE_INCOMPATIBLE(-181),
        ERR_INVALID_ECH_CONFIG_LIST(-182),
        ERR_ECH_NOT_NEGOTIATED(-183),
        ERR_ECH_FALLBACK_CERTIFICATE_INVALID(-184),
        ERR_CERT_COMMON_NAME_INVALID(-200),
        ERR_CERT_DATE_INVALID(-201),
        ERR_CERT_AUTHORITY_INVALID(-202),
        ERR_CERT_CONTAINS_ERRORS(-203),
        ERR_CERT_NO_REVOCATION_MECHANISM(-204),
        ERR_CERT_UNABLE_TO_CHECK_REVOCATION(-205),
        ERR_CERT_REVOKED(-206),
        ERR_CERT_INVALID(-207),
        ERR_CERT_WEAK_SIGNATURE_ALGORITHM(-208),
        ERR_CERT_NON_UNIQUE_NAME(-210),
        ERR_CERT_WEAK_KEY(-211),
        ERR_CERT_NAME_CONSTRAINT_VIOLATION(-212),
        ERR_CERT_VALIDITY_TOO_LONG(-213),
        ERR_CERTIFICATE_TRANSPARENCY_REQUIRED(-214),
        ERR_CERT_SYMANTEC_LEGACY(-215),
        ERR_CERT_KNOWN_INTERCEPTION_BLOCKED(-217),
        ERR_CERT_END(-219),
        ERR_INVALID_URL(-300),
        ERR_DISALLOWED_URL_SCHEME(-301),
        ERR_UNKNOWN_URL_SCHEME(-302),
        ERR_INVALID_REDIRECT(-303),
        ERR_TOO_MANY_REDIRECTS(-310),
        ERR_UNSAFE_REDIRECT(-311),
        ERR_UNSAFE_PORT(-312),
        ERR_INVALID_RESPONSE(-320),
        ERR_INVALID_CHUNKED_ENCODING(-321),
        ERR_METHOD_NOT_SUPPORTED(-322),
        ERR_UNEXPECTED_PROXY_AUTH(-323),
        ERR_EMPTY_RESPONSE(-324),
        ERR_RESPONSE_HEADERS_TOO_BIG(-325),
        ERR_PAC_SCRIPT_FAILED(-327),
        ERR_REQUEST_RANGE_NOT_SATISFIABLE(-328),
        ERR_MALFORMED_IDENTITY(-329),
        ERR_CONTENT_DECODING_FAILED(-330),
        ERR_NETWORK_IO_SUSPENDED(-331),
        ERR_SYN_REPLY_NOT_RECEIVED(-332),
        ERR_ENCODING_CONVERSION_FAILED(-333),
        ERR_UNRECOGNIZED_FTP_DIRECTORY_LISTING_FORMAT(-334),
        ERR_NO_SUPPORTED_PROXIES(-336),
        ERR_HTTP2_PROTOCOL_ERROR(-337),
        ERR_INVALID_AUTH_CREDENTIALS(-338),
        ERR_UNSUPPORTED_AUTH_SCHEME(-339),
        ERR_ENCODING_DETECTION_FAILED(-340),
        ERR_MISSING_AUTH_CREDENTIALS(-341),
        ERR_UNEXPECTED_SECURITY_LIBRARY_STATUS(-342),
        ERR_MISCONFIGURED_AUTH_ENVIRONMENT(-343),
        ERR_UNDOCUMENTED_SECURITY_LIBRARY_STATUS(-344),
        ERR_RESPONSE_BODY_TOO_BIG_TO_DRAIN(-345),
        ERR_RESPONSE_HEADERS_MULTIPLE_CONTENT_LENGTH(-346),
        ERR_INCOMPLETE_HTTP2_HEADERS(-347),
        ERR_PAC_NOT_IN_DHCP(-348),
        ERR_RESPONSE_HEADERS_MULTIPLE_CONTENT_DISPOSITION(-349),
        ERR_RESPONSE_HEADERS_MULTIPLE_LOCATION(-350),
        ERR_HTTP2_SERVER_REFUSED_STREAM(-351),
        ERR_HTTP2_PING_FAILED(-352),
        ERR_CONTENT_LENGTH_MISMATCH(-354),
        ERR_INCOMPLETE_CHUNKED_ENCODING(-355),
        ERR_QUIC_PROTOCOL_ERROR(-356),
        ERR_RESPONSE_HEADERS_TRUNCATED(-357),
        ERR_QUIC_HANDSHAKE_FAILED(-358),
        ERR_HTTP2_INADEQUATE_TRANSPORT_SECURITY(-360),
        ERR_HTTP2_FLOW_CONTROL_ERROR(-361),
        ERR_HTTP2_FRAME_SIZE_ERROR(-362),
        ERR_HTTP2_COMPRESSION_ERROR(-363),
        ERR_PROXY_AUTH_REQUESTED_WITH_NO_CONNECTION(-364),
        ERR_HTTP_1_1_REQUIRED(-365),
        ERR_PROXY_HTTP_1_1_REQUIRED(-366),
        ERR_PAC_SCRIPT_TERMINATED(-367),
        ERR_INVALID_HTTP_RESPONSE(-370),
        ERR_CONTENT_DECODING_INIT_FAILED(-371),
        ERR_HTTP2_RST_STREAM_NO_ERROR_RECEIVED(-372),
        ERR_HTTP2_PUSHED_STREAM_NOT_AVAILABLE(-373),
        ERR_HTTP2_CLAIMED_PUSHED_STREAM_RESET_BY_SERVER(-374),
        ERR_TOO_MANY_RETRIES(-375),
        ERR_HTTP2_STREAM_CLOSED(-376),
        ERR_HTTP2_CLIENT_REFUSED_STREAM(-377),
        ERR_HTTP2_PUSHED_RESPONSE_DOES_NOT_MATCH(-378),
        ERR_HTTP_RESPONSE_CODE_FAILURE(-379),
        ERR_QUIC_CERT_ROOT_NOT_KNOWN(-380),
        ERR_QUIC_GOAWAY_REQUEST_CAN_BE_RETRIED(-381),
        ERR_CACHE_MISS(-400),
        ERR_CACHE_READ_FAILURE(-401),
        ERR_CACHE_WRITE_FAILURE(-402),
        ERR_CACHE_OPERATION_NOT_SUPPORTED(-403),
        ERR_CACHE_OPEN_FAILURE(-404),
        ERR_CACHE_CREATE_FAILURE(-405),
        ERR_CACHE_RACE(-406),
        ERR_CACHE_CHECKSUM_READ_FAILURE(-407),
        ERR_CACHE_CHECKSUM_MISMATCH(-408),
        ERR_CACHE_LOCK_TIMEOUT(-409),
        ERR_CACHE_AUTH_FAILURE_AFTER_READ(-410),
        ERR_CACHE_ENTRY_NOT_SUITABLE(-411),
        ERR_CACHE_DOOM_FAILURE(-412),
        ERR_CACHE_OPEN_OR_CREATE_FAILURE(-413),
        ERR_INSECURE_RESPONSE(-501),
        ERR_NO_PRIVATE_KEY_FOR_CERT(-502),
        ERR_ADD_USER_CERT_FAILED(-503),
        ERR_INVALID_SIGNED_EXCHANGE(-504),
        ERR_INVALID_WEB_BUNDLE(-505),
        ERR_TRUST_TOKEN_OPERATION_FAILED(-506),
        ERR_TRUST_TOKEN_OPERATION_SUCCESS_WITHOUT_SENDING_REQUEST(-507),
        ERR_FTP_FAILED(-601),
        ERR_FTP_SERVICE_UNAVAILABLE(-602),
        ERR_FTP_TRANSFER_ABORTED(-603),
        ERR_FTP_FILE_BUSY(-604),
        ERR_FTP_SYNTAX_ERROR(-605),
        ERR_FTP_COMMAND_NOT_SUPPORTED(-606),
        ERR_FTP_BAD_COMMAND_SEQUENCE(-607),
        ERR_PKCS12_IMPORT_BAD_PASSWORD(-701),
        ERR_PKCS12_IMPORT_FAILED(-702),
        ERR_IMPORT_CA_CERT_NOT_CA(-703),
        ERR_IMPORT_CERT_ALREADY_EXISTS(-704),
        ERR_IMPORT_CA_CERT_FAILED(-705),
        ERR_IMPORT_SERVER_CERT_FAILED(-706),
        ERR_PKCS12_IMPORT_INVALID_MAC(-707),
        ERR_PKCS12_IMPORT_INVALID_FILE(-708),
        ERR_PKCS12_IMPORT_UNSUPPORTED(-709),
        ERR_KEY_GENERATION_FAILED(-710),
        ERR_PRIVATE_KEY_EXPORT_FAILED(-712),
        ERR_SELF_SIGNED_CERT_GENERATION_FAILED(-713),
        ERR_CERT_DATABASE_CHANGED(-714),
        ERR_DNS_MALFORMED_RESPONSE(-800),
        ERR_DNS_SERVER_REQUIRES_TCP(-801),
        ERR_DNS_SERVER_FAILED(-802),
        ERR_DNS_TIMED_OUT(-803),
        ERR_DNS_CACHE_MISS(-804),
        ERR_DNS_SEARCH_EMPTY(-805),
        ERR_DNS_SORT_ERROR(-806),
        ERR_DNS_SECURE_RESOLVER_HOSTNAME_RESOLUTION_FAILED(-808),
        ERR_DNS_NAME_HTTPS_ONLY(-809),
        ERR_DNS_REQUEST_CANCELLED(-810);

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
         * @return The error code as an integer.
         */
        public int getCode() {
            return code;
        }

        /**
         * Finds the ErrorCode by the native chrome embedded framework integer-based
         * error code value.
         * @param code The integer-based raw error code.
         * @return The Java enum mapped to that error code or null if none was found.
         */
        static public ErrorCode findByCode(int code) {
            return CODES.get(code);
        }
    }

    /**
     * Called when the loading state has changed. This callback will be executed twice -- once when
     * loading is initiated either programmatically or by user action, and once when loading is
     * terminated due to completion, cancellation of failure.
     *
     * @param browser The corresponding browser.
     * @param isLoading true if it is loading.
     * @param canGoBack true if you can navigate back.
     * @param canGoForward true if you can navigate forward.
     */
    public void onLoadingStateChange(
            CefBrowser browser, boolean isLoading, boolean canGoBack, boolean canGoForward);

    /**
     * Called when the browser begins loading a frame. The frameIdentifer value will never be empty.
     * Multiple frames may be loading at the same time. Sub-frames may start or continue loading
     * after the main frame load has ended. This method may not be called for a particular frame if
     * the load request for that frame fails. For notification of overall browser load status use
     * onLoadingStateChange instead.
     *
     * @param browser The corresponding browser.
     * @param frame The frame generating the event. Instance only valid within the scope of this
     *         method.
     * @param transitionType The transition type.
     */
    public void onLoadStart(CefBrowser browser, CefFrame frame, TransitionType transitionType);

    /**
     * Called when the browser is done loading a frame. The frameIdentifer value will never be
     * empty. Multiple frames may be loading at the same time. Sub-frames may start or continue
     * loading after the main frame load has ended. This method will always be called for all frames
     * irrespective of whether the request completes successfully.
     *
     * @param browser The corresponding browser.
     * @param frame The frame generating the event. Instance only valid within the scope of this
     *         method.
     * @param httpStatusCode The status code of the load.
     */
    public void onLoadEnd(CefBrowser browser, CefFrame frame, int httpStatusCode);

    /**
     * Called when the resource load for a navigation fails or is canceled.
     *
     * @param browser The corresponding browser.
     * @param frame The frame generating the event. Instance only valid within the scope of this
     *         method.
     * @param errorCode The error code number.
     * @param errorText The error text.
     * @param failedUrl The URL that failed to load.
     */
    public void onLoadError(CefBrowser browser, CefFrame frame, ErrorCode errorCode,
            String errorText, String failedUrl);
}
