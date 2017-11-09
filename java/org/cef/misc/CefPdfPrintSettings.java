// Copyright (c) 2017 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.misc;

/**
 * PDF print settings for browser.printToPDF()
 */
public class CefPdfPrintSettings {
    
    public enum MarginType {
        // Default margins.
        DEFAULT,

        // No margins
        NONE,

        // Minimum margins.
        MINIMUM,

        // Custom margins using the values from CefPdfPrintSettings
        CUSTOM
    }
    
    /**
     * Set to true to print headers and footers or false to not print
     * headers and footers.
     */
    public boolean header_footer_enabled;
    
    /**
     * Page title to display in the header. Only used if header_footer_enabled
     * is set to true.
     */
    public String header_footer_title;

    /**
     * URL to display in the footer. Only used if header_footer_enabled is set
     * to true.
     */
    public String header_footer_url;

    /**
     * Set to true for landscape mode or false for portrait mode.
     */
    public boolean landscape;

    /**
     * Set to true to print background graphics or false to not print
     * background graphics.
     */
    public boolean backgrounds_enabled;
    
    /**
     * Output page size in microns (1 millimeter = 1000 microns). If either of these
     * values is less than or equal to zero then the default paper size will be
     * used as returned by the print_handler.  A4 is 210 × 297 mm which would
     * be 210000 x 297000 microns.  US Letter is 215.9 × 279.4 mm which would 
     * be 215900 x 279400 microns.
     */
    public int page_width;
    public int page_height;
    
    /**
     * Set to true to print the selection only or false to print all.
     */
    public boolean selection_only;
    
    /**
     * The percentage to scale the PDF by before printing (e.g. 50 is 50%).
     * If this value is less than or equal to zero the default value of 100
     * will be used.
     */
    public int scale_factor;
    
    /**
     * Margins in millimeters. Only used if |margin_type| is set to
     * PDF_PRINT_MARGIN_CUSTOM.
     */
    public double margin_top;
    public double margin_right;
    public double margin_bottom;
    public double margin_left;

    /**
     * Margin type.
     */
    public MarginType margin_type;
    
    public CefPdfPrintSettings() {}

    @Override
    public CefPdfPrintSettings clone() {
        CefPdfPrintSettings tmp = new CefPdfPrintSettings();
        tmp.header_footer_enabled = this.header_footer_enabled;
        tmp.header_footer_title = this.header_footer_title;
        tmp.header_footer_url = this.header_footer_url;
        tmp.landscape = this.landscape;
        tmp.backgrounds_enabled = this.backgrounds_enabled;
        tmp.page_width = this.page_width;
        tmp.page_height = this.page_height;
        tmp.selection_only = this.selection_only;
        tmp.scale_factor = this.scale_factor;
        tmp.margin_top = this.margin_top;
        tmp.margin_right = this.margin_right;
        tmp.margin_bottom = this.margin_bottom;
        tmp.margin_left = this.margin_left;
        tmp.margin_type = this.margin_type;
        return tmp;
    }
}
