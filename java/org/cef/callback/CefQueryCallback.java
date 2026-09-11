// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.callback;

import java.nio.ByteBuffer;

/**
 * Interface representing a query callback.
 */
public interface CefQueryCallback {
    /**
     * Notify the associated JavaScript onSuccess callback that the query has
     * completed successfully.
     * @param response Response text passed to JavaScript.
     */
    public void success(String response);

    /**
     * Notify the associated JavaScript onSuccess callback that the query has
     * completed successfully.
     * @param response Response buffer passed to JavaScript. May be null.
     */
    public void success(ByteBuffer response);

    /**
     * Notify the associated JavaScript onFailure callback that the query has
     * failed.
     * @param error_code Error code passed to JavaScript.
     * @param error_message Error message passed to JavaScript.
     */
    public void failure(int error_code, String error_message);

    /**
     * Returns whether this callback is persistent and therefore whether
     * {@link CefQueryCallback#success(String)} may be called multiple times.
     * Persistent queries must be explicitly canceled, either from the browser
     * or via {@link CefQueryCallback#failure(int, String)}.
     */
    public boolean isPersistent();
}
