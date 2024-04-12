// Copyright (c) 2017 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.browser;

/**
 * Interface representing a frame.
 */
public interface CefFrame {
    /**
     * Removes the native reference from an unused object.
     */
    void dispose();

    /**
     * Returns the globally unique identifier for this frame or an empty string if the
     * underlying frame does not yet exist.
     * @return The frame identifier
     */
    String getIdentifier();

    /**
     * Emits the URL currently loaded in this frame.
     * @return the URL currently loaded in this frame.
     */
    String getURL();

    /**
     * Returns the name for this frame. If the frame has an assigned name (for
     * example, set via the iframe "name" attribute) then that value will be
     * returned. Otherwise a unique name will be constructed based on the frame
     * parent hierarchy. The main (top-level) frame will always have an empty name
     * value.
     * @return The frame name
     */
    String getName();

    /**
     * Returns true if this is the main (top-level) frame.
     * @return True if this frame is top-level otherwise false.
     */
    boolean isMain();

    /**
     * True if this object is currently attached to a valid frame.
     * @return True if valid otherwise false.
     */
    boolean isValid();

    /**
     * Returns true if this is the focused frame.
     * @return True if valid otherwise false.
     */
    boolean isFocused();

    /**
     * Returns the parent of this frame or NULL if this is the main (top-level)
     * frame.
     * @return The parent frame or NULL if this is the main frame
     */
    CefFrame getParent();

    /**
     * Execute a string of JavaScript code in this frame. The url
     * parameter is the URL where the script in question can be found, if any.
     * The renderer may request this URL to show the developer the source of the
     * error. The line parameter is the base line number to use for error
     * reporting.
     *
     * @param code The code to be executed.
     * @param url The URL where the script in question can be found.
     * @param line The base line number to use for error reporting.
     */
    public void executeJavaScript(String code, String url, int line);

    /**
     * Execute undo in this frame.
     */
    public void undo();

    /**
     * Execute redo in this frame.
     */
    public void redo();

    /**
     * Execute cut in this frame.
     */
    public void cut();

    /**
     * Execute copy in this frame.
     */
    public void copy();

    /**
     * Execute paste in this frame.
     */
    public void paste();

    /**
     * Execute selectAll in this frame.
     */
    public void selectAll();
}
