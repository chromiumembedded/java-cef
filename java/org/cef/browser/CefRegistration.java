// Copyright (c) 2024 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.browser;

/**
 * Used internally by {@link CefDevToolsClient}.
 * <p>
 * Handle to observer registration, As long as this object is alive, the observer will stay
 * registered.
 */
abstract class CefRegistration {
    /**
     * Removes the native reference from an unused object.
     */
    public abstract void dispose();

    @Override
    public void finalize() throws Throwable {
        dispose();
        super.finalize();
    }
}
