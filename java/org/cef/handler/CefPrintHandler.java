// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.handler;

import org.cef.browser.CefBrowser;
import org.cef.callback.CefNative;
import org.cef.callback.CefPrintDialogCallback;
import org.cef.callback.CefPrintJobCallback;
import org.cef.misc.CefPrintSettings;

import java.awt.Dimension;

/**
 * Implement this interface to handle printing on Linux. The methods of this class will be called on
 * the browser process UI thread.
 */
public interface CefPrintHandler extends CefNative {
    /**
     * Called when printing has started. This method will be called before the other onPrint*()
     * methods and irrespective of how printing was initiated (e.g. CefBrowser::print(), JavaScript
     * window.print() or PDF extension print button).
     *
     * @param browser The corresponding browser.
     */
    void onPrintStart(CefBrowser browser);

    /**
     * Called to get print settings.
     *
     * @param browser The corresponding browser.
     * @param settings Populate with the desired print settings. Do not keep a reference to this
     *         object outside of this callback.
     * @param getDefaults If true |settings| should be populated with the default print settings.
     */
    void onPrintSettings(CefBrowser browser, CefPrintSettings settings, boolean getDefaults);

    /**
     * Called to show the print dialog.
     *
     * @param browser The corresponding browser.
     * @param hasSelection True if the user has selected a region of the page to print.
     * @param callback Callback to execute after the dialog is dismissed.
     * @return True if the dialog will be displayed or false to cancel the printing immediately.
     */
    boolean onPrintDialog(
            CefBrowser browser, boolean hasSelection, CefPrintDialogCallback callback);

    /**
     * Called to send the print job to the printer.
     *
     * @param browser The corresponding browser.
     * @param documentName Name of the document that is printing.
     * @param pdfFilePath Path to the PDF file that contains the document contents.
     * @param callback Callback to execute after the print job has completed.
     * @return True if the job will proceed or false to cancel the printing immediately.
     */
    boolean onPrintJob(CefBrowser browser, String documentName, String pdfFilePath,
            CefPrintJobCallback callback);

    /**
     * Called to reset client state related to printing.
     *
     * @param browser The corresponding browser.
     */
    void onPrintReset(CefBrowser browser);

    /**
     * Called to retrieve the page size when printToPDF is requested for a browser.
     *
     * @param browser The corresponding browser.
     * @param deviceUnitsPerInch The DPI of the print. Use this to calculate the page size to use.
     * @return The page size in microns.
     */
    Dimension getPdfPaperSize(CefBrowser browser, int deviceUnitsPerInch);
}
