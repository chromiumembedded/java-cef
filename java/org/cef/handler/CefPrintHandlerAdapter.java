// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.handler;

import java.awt.Dimension;
import org.cef.browser.CefBrowser;
import org.cef.callback.CefNativeAdapter;
import org.cef.callback.CefPrintDialogCallback;
import org.cef.callback.CefPrintJobCallback;
import org.cef.misc.CefPrintSettings;

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
    public void onPrintSettings(CefPrintSettings settings, boolean get_defaults) {
        // The default implementation does nothing
    }

    @Override
    public boolean onPrintDialog(boolean has_selection, CefPrintDialogCallback callback) {
        // The default implementation does nothing
        return false;
    }

    @Override
    public boolean onPrintJob(
            String document_name, String pdf_file_path, CefPrintJobCallback callback) {
        // The default implementation does nothing
        return false;
    }

    @Override
    public void onPrintReset() {
        // The default implementation does nothing
    }
    
    @Override
    public Dimension getPdfPaperSize(int deviceUnitsPerInch) {
        // default implementation is A4 letter size
        // @ 300 DPI, A4 is 2480 x 3508
        // @ 150 DPI, A4 is 1240 x 1754
        int adjustedWidth = (int)(((double)deviceUnitsPerInch/300d)*2480d);
        int adjustedHeight = (int)(((double)deviceUnitsPerInch/300d)*3508d);
        return new Dimension(adjustedWidth, adjustedHeight);
    }
}
