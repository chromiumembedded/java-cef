// Copyright (c) 2017 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.callback;

/**
 * Callback interface for CefBrowser.PrintToPDF(). The methods of this class
 * will be called on the browser process UI thread.
 */
public interface CefPdfPrintCallback {
    
    /**
     * Method that will be executed when the PDF printing has completed. |path|
     * is the output path. |ok| will be true if the printing completed
     * successfully or false otherwise.
     * @param path The path of the PDF file that was written.
     * @param ok True if printing completed or false otherwise.
     */
    public abstract void onPdfPrintFinished(String path, boolean ok);
    
}
