// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.handler;

import org.cef.browser.CefBrowser;
import org.cef.callback.CefNativeAdapter;
import org.cef.callback.CefPrintDialogCallback;
import org.cef.callback.CefPrintJobCallback;
import org.cef.misc.CefPrintSettings;

import java.awt.Dimension;

/**
 * An abstract adapter class for receiving print events on Linux.
 * The methods in this class are empty.
 * This class exists as convenience for creating handler objects.
 */
public abstract class CefPrintHandlerAdapter extends CefNativeAdapter implements CefPrintHandler {
    @Override
    public void onPrintStart(CefBrowser browser) {
        // The default implementation does nothing
    }

    @Override
    public void onPrintSettings(
            CefBrowser browser, CefPrintSettings settings, boolean getDefaults) {
        // The default implementation does nothing
    }

    @Override
    public boolean onPrintDialog(
            CefBrowser browser, boolean hasSelection, CefPrintDialogCallback callback) {
        // The default implementation does nothing
        return false;
    }

    @Override
    public boolean onPrintJob(CefBrowser browser, String documentName, String pdfFilePath,
            CefPrintJobCallback callback) {
        // The default implementation does nothing
        return false;
    }

    @Override
    public void onPrintReset(CefBrowser browser) {
        // The default implementation does nothing
    }

    @Override
    public Dimension getPdfPaperSize(CefBrowser browser, int deviceUnitsPerInch) {
        // default implementation is A4 letter size
        // @ 300 DPI, A4 is 2480 x 3508
        // @ 150 DPI, A4 is 1240 x 1754
        int adjustedWidth = (int) (((double) deviceUnitsPerInch / 300d) * 2480d);
        int adjustedHeight = (int) (((double) deviceUnitsPerInch / 300d) * 3508d);
        return new Dimension(adjustedWidth, adjustedHeight);
    }
}
