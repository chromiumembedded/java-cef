// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.handler;

import org.cef.callback.CefCallback;
import org.cef.misc.IntRef;
import org.cef.misc.StringRef;
import org.cef.network.CefCookie;
import org.cef.network.CefRequest;
import org.cef.network.CefResponse;

/**
 * Implement this interface to handle custom resource requests. The methods of
 * this class will always be called on the IO thread.
 */
public interface CefResourceHandler {
    /**
     * Begin processing the request.
     * @param request The request itself. Cannot be modified in this callback. Instance only valid
     *         within the scope of this method.
     * @param callback Callback to continue or cancel the request.
     * @return True to handle the request and call CefCallback.Continue() once the response header
     *         information is available.
     */
    boolean processRequest(CefRequest request, CefCallback callback);

    /**
     * Retrieve response header information. If the response length is not known set
     * |responseLength| to -1 and readResponse() will be called until it returns false. If the
     * response length is known set |responseLength| to a positive value and readResponse() will be
     * called until it returns false or the specified number of bytes have been read. Use the
     * |response| object to set the mime type, http status code and other optional header values.
     * @param response The request response that should be returned. Instance only valid within the
     *         scope of this method.
     * @param responseLength Optionally set the response length if known.
     * @param redirectUrl Optionally redirect the request to a new URL.
     */
    void getResponseHeaders(CefResponse response, IntRef responseLength, StringRef redirectUrl);

    /**
     * Read response data. If data is available immediately copy up to |bytesToRead| bytes into
     * |dataOut|, set |bytesRead| to the number of bytes copied, and return true. To read the data
     * at a later time set |bytesRead| to 0, return true and call CefCallback.Continue() when the
     * data is available. To indicate response completion return false.
     * @param dataOut Write data to this buffer.
     * @param bytesToRead Size of the buffer.
     * @param bytesRead Number of bytes written to the buffer.
     * @param callback Callback to execute if data will be available asynchronously.
     * @return True if more data is or will be available.
     */
    boolean readResponse(byte[] dataOut, int bytesToRead, IntRef bytesRead, CefCallback callback);

    /**
     * Request processing has been canceled.
     */
    void cancel();
}
