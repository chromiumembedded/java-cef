// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.network;

import java.util.HashMap;
import java.util.Map;
import java.util.Map.Entry;
import java.util.Set;

/**
 * Class used to represent a web request. The methods of this class may be
 * called on any thread.
 */
public abstract class CefRequest {
    /**
     * Resource type for a request.
     */
    public enum ResourceType {
        RT_MAIN_FRAME, //!< Top level page.
        RT_SUB_FRAME, //!< Frame or iframe.
        RT_STYLESHEET, //!< CSS stylesheet.
        RT_SCRIPT, //!< External script.
        RT_IMAGE, //!< Image (jpg/gif/png/etc).
        RT_FONT_RESOURCE, //!< Font.
        RT_SUB_RESOURCE, //!< Some other subresource. This is the default type if
                         //   the actual type is unknown.
        RT_OBJECT, //!< Object (or embed) tag for a plugin, or a resource
                   //   that a plugin requested.
        RT_MEDIA, //!< Media resource.
        RT_WORKER, //!< Main resource of a dedicated worker.
        RT_SHARED_WORKER, //!< Main resource of a shared worker.
        RT_PREFETCH, //!< Explicitly requested prefetch.
        RT_FAVICON, //!< Favicon.
        RT_XHR, //!< XMLHttpRequest.
        RT_PING, //!< A request for a <ping>
        RT_SERVICE_WORKER, //!< Main resource of a service worker.
        RT_CSP_REPORT, //!< A report of Content Security Policy violations.
        RT_PLUGIN_RESOURCE, //!< A resource that a plugin requested.
        RT_NAVIGATION_PRELOAD_MAIN_FRAME, //!< A main-frame service worker navigation preload
                                          //!< request.
        RT_NAVIGATION_PRELOAD_SUB_FRAME, //!< A sub-frame service worker navigation preload request.
    }

    /**
     * Transition type for a request. Made up of one source value and 0 or more
     * qualifiers.
     */
    public enum TransitionType {
        /**
         * Source is a link click or the JavaScript window.open function. This is
         * also the default value for requests like sub-resource loads that are not
         * navigations.
         */
        TT_LINK(0),

        /**
         * Source is some other "explicit" navigation action such as creating a new
         * browser or using the LoadURL function. This is also the default value
         * for navigations where the actual type is unknown.
         */
        TT_EXPLICIT(1),

        /**
         * Source is a subframe navigation. This is any content that is automatically
         * loaded in a non-toplevel frame. For example, if a page consists of several
         * frames containing ads, those ad URLs will have this transition type.
         * The user may not even realize the content in these pages is a separate
         * frame, so may not care about the URL.
         */
        TT_AUTO_SUBFRAME(3),

        /**
         * Source is a subframe navigation explicitly requested by the user that will
         * generate new navigation entries in the back/forward list. These are
         * probably more important than frames that were automatically loaded in
         * the background because the user probably cares about the fact that this
         * link was loaded.
         */
        TT_MANUAL_SUBFRAME(4),

        /**
         * Source is a form submission by the user. NOTE: In some situations
         * submitting a form does not result in this transition type. This can happen
         * if the form uses a script to submit the contents.
         */
        TT_FORM_SUBMIT(7),

        /**
         * Source is a "reload" of the page via the Reload function or by re-visiting
         * the same URL. NOTE: This is distinct from the concept of whether a
         * particular load uses "reload semantics" (i.e. bypasses cached data).
         */
        TT_RELOAD(8);

        private int value;
        private TransitionType(int source) {
            value = source;
        }

        /**
         * Returns the integer representation of this enum, containing the source
         * and the qualifier as one value.
         * @return The integer value of the enum with all its qualifiers.
         */
        public int getValue() {
            return value;
        }

        /**
         * Returns the source part of the enum as integer.
         * @return Integer representation of the set source.
         */
        public int getSource() {
            return (value & 0xFF);
        }

        /**
         * Any of the core values above can be augmented by one or more qualifiers
         * defined as TransitionFlags.
         * These qualifiers further define the transition.
         */
        public void addQualifier(TransitionFlags flag) {
            value |= flag.getValue();
        }

        /**
         * Add qualifiers as integer value
         */
        public void addQualifiers(int flags) {
            value |= (flags & 0xFFFFFF00);
        }

        /**
         * Returns the qualifier part of the enum as integer.
         * @return Integer representation of the set qualifiers.
         */
        public int getQualifiers() {
            return (value & 0xFFFFFF00);
        }

        /**
         * Removes a qualifier from the enum.
         * @param The qualifier to be removed.
         */
        public void removeQualifier(TransitionFlags flag) {
            value &= ~flag.getValue();
        }

        /**
         * Tests if a qualifier is set.
         */
        public boolean isSet(TransitionFlags flag) {
            return (value & flag.getValue()) != 0;
        }

        /**
         * Tests if one of the redirect qualifiers is set.
         */
        public boolean isRedirect() {
            return (value & 0xC0000000) != 0;
        }
    }

    public static final class CefUrlRequestFlags {
        /**
         * Default behavior.
         */
        public static final int UR_FLAG_NONE = 0;

        /**
         * If set the cache will be skipped when handling the request.
         */
        public static final int UR_FLAG_SKIP_CACHE = 1 << 0;

        /**
         * If set user name, password, and cookies may be sent with the request, and
         * cookies may be saved from the response.
         */
        public static final int UR_FLAG_ALLOW_CACHED_CREDENTIALS = 1 << 1;

        /**
         * If set upload progress events will be generated when a request has a body.
         */
        public static final int UR_FLAG_REPORT_UPLOAD_PROGRESS = 1 << 3;

        /**
         * If set the headers sent and received for the request will be recorded.
         */
        public static final int UR_FLAG_REPORT_RAW_HEADERS = 1 << 5;

        /**
         *  If set the CefURLRequestClient.onDownloadData method will not be called.
         */
        public static final int UR_FLAG_NO_DOWNLOAD_DATA = 1 << 6;

        /**
         * If set 5XX redirect errors will be propagated to the observer instead of
         * automatically re-tried. This currently only applies for requests
         * originated in the browser process.
         */
        public static final int UR_FLAG_NO_RETRY_ON_5XX = 1 << 7;
    }

    /**
     * Transition qualifiers.
     * Any of the core values above can be augmented by one or more qualifiers.
     * These qualifiers further define the transition.
     */
    public enum TransitionFlags {
        /**
         * Attempted to visit a URL but was blocked.
         */
        TT_BLOCKED_FLAG(0x00800000),

        /**
         * Used the Forward or Back function to navigate among browsing history.
         */
        TT_FORWARD_BACK_FLAG(0x01000000),

        /**
         * The beginning of a navigation chain.
         */
        TT_CHAIN_START_FLAG(0x10000000),

        /**
         * The last transition in a redirect chain.
         */
        TT_CHAIN_END_FLAG(0x20000000),

        /**
         * Redirects caused by JavaScript or a meta refresh tag on the page.
         */
        TT_CLIENT_REDIRECT_FLAG(0x40000000),

        /**
         * Redirects sent from the server by HTTP headers.
         */
        TT_SERVER_REDIRECT_FLAG(0x80000000);

        private final int flag;
        private TransitionFlags(int flag) {
            this.flag = flag;
        }

        /**
         * Returns the integer representation of the enum.
         * @return Integer representation of the enum.
         */
        public int getValue() {
            return flag;
        }
    }

    /**
     * Policy for how the Referrer HTTP header value will be sent during navigation.
     * If the `--no-referrers` command-line flag is specified then the policy value
     * will be ignored and the Referrer value will never be sent.
     */
    public enum ReferrerPolicy {

        /**
         * This is the same as REFERRER_POLICY_CLEAR_REFERRER_ON_TRANSITION_FROM_SECURE_TO_INSECURE
         * but here to match CEF native.
         */
        REFERRER_POLICY_DEFAULT,

        /**
         * Clear the referrer header if the header value is HTTPS but the request
         * destination is HTTP. This is the default behavior.
         */
        REFERRER_POLICY_CLEAR_REFERRER_ON_TRANSITION_FROM_SECURE_TO_INSECURE,

        /**
         * A slight variant on CLEAR_REFERRER_ON_TRANSITION_FROM_SECURE_TO_INSECURE:
         * If the request destination is HTTP, an HTTPS referrer will be cleared. If
         * the request's destination is cross-origin with the referrer (but does not
         * downgrade), the referrer's granularity will be stripped down to an origin
         * rather than a full URL. Same-origin requests will send the full referrer.
         */
        REFERRER_POLICY_REDUCE_REFERRER_GRANULARITY_ON_TRANSITION_CROSS_ORIGIN,

        /**
         * Strip the referrer down to an origin when the origin of the referrer is
         * different from the destination's origin.
         */
        REFERRER_POLICY_ORIGIN_ONLY_ON_TRANSITION_CROSS_ORIGIN,

        /**
         * Never change the referrer.
         */
        REFERRER_POLICY_NEVER_CLEAR_REFERRER,

        /**
         * Strip the referrer down to the origin regardless of the redirect location.
         */
        REFERRER_POLICY_ORIGIN,

        /**
         * Clear the referrer when the request's referrer is cross-origin with the
         * request destination.
         */
        REFERRER_POLICY_CLEAR_REFERRER_ON_TRANSITION_CROSS_ORIGIN,

        /**
         * Strip the referrer down to the origin, but clear it entirely if the
         * referrer value is HTTPS and the destination is HTTP.
         */
        REFERRER_POLICY_ORIGIN_CLEAR_ON_TRANSITION_FROM_SECURE_TO_INSECURE,

        /**
         * Always clear the referrer regardless of the request destination.
         */
        REFERRER_POLICY_NO_REFERRER,

        /**
         * Always the last value in this enumeration.
         */
        REFERRER_POLICY_LAST_VALUE
    }

    // This CTOR can't be called directly. Call method create() instead.
    CefRequest() {}

    @Override
    protected void finalize() throws Throwable {
        dispose();
        super.finalize();
    }

    /**
     * Create a new CefRequest object.
     */
    public static final CefRequest create() {
        return CefRequest_N.createNative();
    }

    /**
     * Removes the native reference from an unused object.
     */
    public abstract void dispose();

    /**
     * Returns the globally unique identifier for this request or 0 if not
     * specified. Can be used by CefRequestHandler implementations in the browser
     * process to track a single request across multiple callbacks.
     */
    public abstract long getIdentifier();

    /**
     * Returns true if this object is read-only.
     */
    public abstract boolean isReadOnly();

    /**
     * Get the fully qualified URL.
     */
    public abstract String getURL();

    /**
     * Set the fully qualified URL.
     */
    public abstract void setURL(String url);

    /**
     * Get the request method type. The value will default to POST if post data
     * is provided and GET otherwise.
     */
    public abstract String getMethod();

    /**
     * Set the request method type.
     */
    public abstract void setMethod(String method);

    /**
     * Set the referrer URL and policy. If non-empty the referrer URL must be
     * fully qualified with an HTTP or HTTPS scheme component. Any username,
     * password or ref component will be removed.
     */
    public abstract void setReferrer(String url, ReferrerPolicy policy);

    /**
     * Get the referrer URL.
     */
    public abstract String getReferrerURL();

    /**
     * Get the referrer policy.
     */
    public abstract ReferrerPolicy getReferrerPolicy();

    /**
     * Get the post data.
     */
    public abstract CefPostData getPostData();

    /**
     * Set the post data.
     */
    public abstract void setPostData(CefPostData postData);

    /**
     * Get the value for the specified response header field. The Referer value cannot be retrieved
     * using this method. Use getHeaderMap instead if there might be multiple values.
     * @param name The header name.
     * @return The header value.
     */
    public abstract String getHeaderByName(String name);

    /**
     * Set the value for the specified response header field. The Referer value cannot be set using
     * this method.
     * @param name The header name.
     * @param value The header value.
     * @param overwrite If true any existing values will be replaced with the new value. If false
     *         any existing values will not be overwritten.
     */
    public abstract void setHeaderByName(String name, String value, boolean overwrite);

    /**
     * Get the header values.
     */
    public abstract void getHeaderMap(Map<String, String> headerMap);

    /**
     * Set the header values.
     */
    public abstract void setHeaderMap(Map<String, String> headerMap);

    /**
     * Set all values at one time.
     */
    public abstract void set(
            String url, String method, CefPostData postData, Map<String, String> headerMap);

    /**
     * Get the flags used in combination with CefURLRequest. See
     * CefUrlRequestFlags for supported values.
     */
    public abstract int getFlags();

    /**
     * Set the flags used in combination with CefURLRequest. See
     * CefUrlRequestFlags for supported values.
     */
    public abstract void setFlags(int flags);

    /**
     * Get the URL to the first party for cookies used in combination with
     * CefURLRequest.
     */
    public abstract String getFirstPartyForCookies();

    /**
     * Set the URL to the first party for cookies used in combination with
     * CefURLRequest.
     */
    public abstract void setFirstPartyForCookies(String url);

    /**
     * Get the resource type for this request. Accurate resource type information
     * may only be available in the browser process.
     */
    public abstract ResourceType getResourceType();

    /**
     * Get the transition type for this request. Only available in the browser
     * process and only applies to requests that represent a main frame or
     * sub-frame navigation.
     */
    public abstract TransitionType getTransitionType();

    @Override
    public String toString() {
        String returnValue = "\nHTTP-Request";
        returnValue += "\n  flags: " + getFlags();
        returnValue += "\n  resourceType: " + getResourceType();
        returnValue += "\n  transitionType: " + getTransitionType();
        returnValue += "\n  firstPartyForCookies: " + getFirstPartyForCookies();
        returnValue += "\n  referrerURL: " + getReferrerURL();
        returnValue += "\n  referrerPolicy: " + getReferrerPolicy();
        returnValue += "\n    " + getMethod() + " " + getURL() + " HTTP/1.1\n";

        Map<String, String> headerMap = new HashMap<>();
        getHeaderMap(headerMap);
        Set<Entry<String, String>> entrySet = headerMap.entrySet();
        String mimeType = null;
        for (Entry<String, String> entry : entrySet) {
            String key = entry.getKey();
            returnValue += "    " + key + "=" + entry.getValue() + "\n";
            if (key.equals("Content-Type")) {
                mimeType = entry.getValue();
            }
        }

        CefPostData pd = getPostData();
        if (pd != null) {
            returnValue += pd.toString(mimeType);
        }

        return returnValue;
    }
}
