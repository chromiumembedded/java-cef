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

        // Custom margins using the values from CefPdfPrintSettings
        CUSTOM
    }

    /**
     * Set to true for landscape mode or false for portrait mode.
     */
    public boolean landscape;

    /**
     * Set to true to print background graphics or false to not print
     * background graphics.
     */
    public boolean print_background;

    /**
     * The percentage to scale the PDF by before printing (e.g. .5 is 50%).
     * If this value is less than or equal to zero the default value of 1.0
     * will be used.
     */
    public double scale;

    /**
     * Output paper size in inches. If either of these values is less than or
     * equal to zero then the default paper size (letter, 8.5 x 11 inches) will
     * be used.
     */
    public double paper_width;
    public double paper_height;

    /**
     * Set to true to prefer page size as defined by css. Defaults to false
     * in which case the content will be scaled to fit the paper size.
     */
    public boolean prefer_css_page_size;

    /**
     * Margin type.
     */
    public MarginType margin_type;

    /**
     * Margins in inches. Only used if margin_type is set to CUSTOM.
     */
    public double margin_top;
    public double margin_right;
    public double margin_bottom;
    public double margin_left;

    /**
     * Paper ranges to print, one based, e.g., '1-5, 8, 11-13'. Pages are printed
     * in the document order, not in the order specified, and no more than once.
     * Defaults to empty string, which implies the entire document is printed.
     * The page numbers are quietly capped to actual page count of the document,
     * and ranges beyond the end of the document are ignored. If this results in
     * no pages to print, an error is reported. It is an error to specify a range
     * with start greater than end.
     */
    public String page_ranges;

    /**
     * Set to true to print headers and footers or false to not print
     * headers and footers. Modify header_template and/or footer_template to
     * customize the display.
     */
    public boolean display_header_footer;

    /**
     * HTML template for the print header. Only displayed if
     * |display_header_footer| is true (1). Should be valid HTML markup with
     * the following classes used to inject printing values into them:
     *
     * - date: formatted print date
     * - title: document title
     * - url: document location
     * - pageNumber: current page number
     * - totalPages: total pages in the document
     *
     * For example, "<span class=title></span>" would generate a span containing
     * the title.
     */
    public String header_template;

    /**
     * HTML template for the print footer. Only displayed if
     * |display_header_footer| is true (1). Uses the same format as
     * |header_template|.
     */
    public String footer_template;

    /**
     * Whether or not to generate tagged (accessible) PDF.
     */
    public boolean generate_tagged_pdf;

    /**
     * Whether or not to embed the document outline into the PDF.
     */
    public boolean generate_document_outline;

    public CefPdfPrintSettings() {}

    @Override
    public CefPdfPrintSettings clone() {
        CefPdfPrintSettings tmp = new CefPdfPrintSettings();
        tmp.landscape = this.landscape;
        tmp.print_background = this.print_background;
        tmp.scale = this.scale;
        tmp.paper_width = this.paper_width;
        tmp.paper_height = this.paper_height;
        tmp.prefer_css_page_size = this.prefer_css_page_size;
        tmp.margin_type = this.margin_type;
        tmp.margin_top = this.margin_top;
        tmp.margin_bottom = this.margin_bottom;
        tmp.margin_right = this.margin_right;
        tmp.margin_left = this.margin_left;
        tmp.page_ranges = this.page_ranges;
        tmp.display_header_footer = this.display_header_footer;
        tmp.header_template = this.header_template;
        tmp.footer_template = this.footer_template;
        tmp.generate_tagged_pdf = this.generate_tagged_pdf;
        tmp.generate_document_outline = this.generate_document_outline;
        return tmp;
    }
}
