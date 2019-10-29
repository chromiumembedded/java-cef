// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.misc;

import org.cef.callback.CefNative;

import java.awt.Dimension;
import java.awt.Rectangle;
import java.util.Vector;

class CefPrintSettings_N extends CefPrintSettings implements CefNative {
    // Used internally to store a pointer to the CEF object.
    private long N_CefHandle = 0;

    @Override
    public void setNativeRef(String identifer, long nativeRef) {
        N_CefHandle = nativeRef;
    }

    @Override
    public long getNativeRef(String identifer) {
        return N_CefHandle;
    }

    CefPrintSettings_N() {
        super();
    }

    public static CefPrintSettings createNative() {
        try {
            return CefPrintSettings_N.N_Create();
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
            return null;
        }
    }

    @Override
    public void dispose() {
        try {
            N_Dispose(N_CefHandle);
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
    }

    @Override
    public boolean isValid() {
        try {
            return N_IsValid(N_CefHandle);
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
        return false;
    }

    @Override
    public boolean isReadOnly() {
        try {
            return N_IsReadOnly(N_CefHandle);
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
        return false;
    }

    @Override
    public void setOrientation(boolean landscape) {
        try {
            N_SetOrientation(N_CefHandle, landscape);
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
    }

    @Override
    public boolean isLandscape() {
        try {
            return N_IsLandscape(N_CefHandle);
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
        return false;
    }

    @Override
    public void setPrinterPrintableArea(Dimension physical_size_device_units,
            Rectangle printable_area_device_units, boolean landscape_needs_flip) {
        try {
            N_SetPrinterPrintableArea(N_CefHandle, physical_size_device_units,
                    printable_area_device_units, landscape_needs_flip);
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
    }

    @Override
    public void setDeviceName(String name) {
        try {
            N_SetDeviceName(N_CefHandle, name);
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
    }

    @Override
    public String getDeviceName() {
        try {
            return N_GetDeviceName(N_CefHandle);
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
        return null;
    }

    @Override
    public void setDPI(int dpi) {
        try {
            N_SetDPI(N_CefHandle, dpi);
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
    }

    @Override
    public int getDPI() {
        try {
            return N_GetDPI(N_CefHandle);
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
        return 0;
    }

    @Override
    public void setPageRanges(Vector<CefPageRange> ranges) {
        try {
            N_SetPageRanges(N_CefHandle, ranges);
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
    }

    @Override
    public int getPageRangesCount() {
        try {
            return N_GetPageRangesCount(N_CefHandle);
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
        return 0;
    }

    @Override
    public void getPageRanges(Vector<CefPageRange> ranges) {
        try {
            N_GetPageRanges(N_CefHandle, ranges);
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
    }

    @Override
    public void setSelectionOnly(boolean selection_only) {
        try {
            N_SetSelectionOnly(N_CefHandle, selection_only);
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
    }

    @Override
    public boolean isSelectionOnly() {
        try {
            return N_IsSelectionOnly(N_CefHandle);
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
        return false;
    }

    @Override
    public void setCollate(boolean collate) {
        try {
            N_SetCollate(N_CefHandle, collate);
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
    }

    @Override
    public boolean willCollate() {
        try {
            return N_WillCollate(N_CefHandle);
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
        return false;
    }

    @Override
    public void setColorModel(ColorModel model) {
        try {
            N_SetColorModel(N_CefHandle, model);
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
    }

    @Override
    public ColorModel getColorModel() {
        try {
            return N_GetColorModel(N_CefHandle);
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
        return null;
    }

    @Override
    public void setCopies(int copies) {
        try {
            N_SetCopies(N_CefHandle, copies);
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
    }

    @Override
    public int getCopies() {
        try {
            return N_GetCopies(N_CefHandle);
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
        return 0;
    }

    @Override
    public void setDuplexMode(DuplexMode mode) {
        try {
            N_SetDuplexMode(N_CefHandle, mode);
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
    }

    @Override
    public DuplexMode getDuplexMode() {
        try {
            return N_GetDuplexMode(N_CefHandle);
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
        return null;
    }

    private final native static CefPrintSettings_N N_Create();
    private final native void N_Dispose(long self);
    private final native boolean N_IsValid(long self);
    private final native boolean N_IsReadOnly(long self);
    private final native void N_SetOrientation(long self, boolean landscape);
    private final native boolean N_IsLandscape(long self);
    private final native void N_SetPrinterPrintableArea(long self,
            Dimension physical_size_device_units, Rectangle printable_area_device_units,
            boolean landscape_needs_flip);
    private final native void N_SetDeviceName(long self, String name);
    private final native String N_GetDeviceName(long self);
    private final native void N_SetDPI(long self, int dpi);
    private final native int N_GetDPI(long self);
    private final native void N_SetPageRanges(long self, Vector<CefPageRange> ranges);
    private final native int N_GetPageRangesCount(long self);
    private final native void N_GetPageRanges(long self, Vector<CefPageRange> ranges);
    private final native void N_SetSelectionOnly(long self, boolean selection_only);
    private final native boolean N_IsSelectionOnly(long self);
    private final native void N_SetCollate(long self, boolean collate);
    private final native boolean N_WillCollate(long self);
    private final native void N_SetColorModel(long self, ColorModel model);
    private final native ColorModel N_GetColorModel(long self);
    private final native void N_SetCopies(long self, int copies);
    private final native int N_GetCopies(long self);
    private final native void N_SetDuplexMode(long self, DuplexMode mode);
    private final native DuplexMode N_GetDuplexMode(long self);
}
