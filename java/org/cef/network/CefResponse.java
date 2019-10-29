// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.network;

import org.cef.handler.CefLoadHandler.ErrorCode;

import java.util.HashMap;
import java.util.Map;
import java.util.Map.Entry;
import java.util.Set;

/**
 * Class used to represent a web response. The methods of this class may be
 * called on any thread.
 */
public abstract class CefResponse {
    // This CTOR can't be called directly. Call method create() instead.
    CefResponse() {}

    @Override
    protected void finalize() throws Throwable {
        dispose();
        super.finalize();
    }

    /**
     * Create a new CefRequest object.
     */
    public static final CefResponse create() {
        return CefResponse_N.createNative();
    }

    /**
     * Removes the native reference from an unused object.
     */
    public abstract void dispose();

    /**
     * Returns true if this object is read-only.
     */
    public abstract boolean isReadOnly();

    /**
     * Get the response error code. Returns ERR_NONE if there was no error.
     */
    public abstract ErrorCode getError();

    /**
     * Get the response error code. Returns ERR_NONE if there was no error.
     */
    public abstract void setError(ErrorCode errorCode);

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
     * Get the value for the specified response header field. Use getHeaderMap instead if there
     * might be multiple values.
     * @param name The header name.
     * @return The header value.
     */
    public abstract String getHeaderByName(String name);

    /**
     * Set the value for the specified response header field.
     * @param name The header name.
     * @param value The header value.
     * @param overwrite If true any existing values will be replaced with the new value. If false
     *         any existing values will not be overwritten.
     */
    public abstract void setHeaderByName(String name, String value, boolean overwrite);

    /**
     * Get all response header fields.
     */
    public abstract void getHeaderMap(Map<String, String> headerMap);

    /**
     * Set all response header fields.
     */
    public abstract void setHeaderMap(Map<String, String> headerMap);

    @Override
    public String toString() {
        String returnValue = "\nHTTP-Response:";

        returnValue += "\n  error: " + getError();
        returnValue += "\n  readOnly: " + isReadOnly();
        returnValue += "\n    HTTP/1.1 " + getStatus() + " " + getStatusText();
        returnValue += "\n    Content-Type: " + getMimeType();

        Map<String, String> headerMap = new HashMap<>();
        getHeaderMap(headerMap);
        Set<Entry<String, String>> entrySet = headerMap.entrySet();
        for (Entry<String, String> entry : entrySet) {
            returnValue += "    " + entry.getKey() + "=" + entry.getValue() + "\n";
        }

        return returnValue;
    }
}
