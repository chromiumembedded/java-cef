// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.callback;

import java.util.Vector;

class CefContextMenuParams_N extends CefNativeAdapter implements CefContextMenuParams {
    CefContextMenuParams_N() {}

    @Override
    public int getXCoord() {
        try {
            return N_GetXCoord(getNativeRef(null));
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
        return 0;
    }

    @Override
    public int getYCoord() {
        try {
            return N_GetYCoord(getNativeRef(null));
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
        return 0;
    }

    @Override
    public int getTypeFlags() {
        try {
            return N_GetTypeFlags(getNativeRef(null));
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
        return 0;
    }

    @Override
    public String getLinkUrl() {
        try {
            return N_GetLinkUrl(getNativeRef(null));
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
        return null;
    }

    @Override
    public String getUnfilteredLinkUrl() {
        try {
            return N_GetUnfilteredLinkUrl(getNativeRef(null));
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
        return null;
    }

    @Override
    public String getSourceUrl() {
        try {
            return N_GetSourceUrl(getNativeRef(null));
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
        return null;
    }

    @Override
    public boolean hasImageContents() {
        try {
            return N_HasImageContents(getNativeRef(null));
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
        return false;
    }

    @Override
    public String getPageUrl() {
        try {
            return N_GetPageUrl(getNativeRef(null));
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
        return null;
    }

    @Override
    public String getFrameUrl() {
        try {
            return N_GetFrameUrl(getNativeRef(null));
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
        return null;
    }

    @Override
    public String getFrameCharset() {
        try {
            return N_GetFrameCharset(getNativeRef(null));
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
        return null;
    }

    @Override
    public MediaType getMediaType() {
        try {
            return N_GetMediaType(getNativeRef(null));
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
        return null;
    }

    @Override
    public int getMediaStateFlags() {
        try {
            return N_GetMediaStateFlags(getNativeRef(null));
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
        return 0;
    }

    @Override
    public String getSelectionText() {
        try {
            return N_GetSelectionText(getNativeRef(null));
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
        return null;
    }

    @Override
    public String getMisspelledWord() {
        try {
            return N_GetMisspelledWord(getNativeRef(null));
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
        return null;
    }

    @Override
    public boolean getDictionarySuggestions(Vector<String> suggestions) {
        try {
            return N_GetDictionarySuggestions(getNativeRef(null), suggestions);
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
        return false;
    }

    @Override
    public boolean isEditable() {
        try {
            return N_IsEditable(getNativeRef(null));
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
        return false;
    }

    @Override
    public boolean isSpellCheckEnabled() {
        try {
            return N_IsSpellCheckEnabled(getNativeRef(null));
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
        return false;
    }

    @Override
    public int getEditStateFlags() {
        try {
            return N_GetEditStateFlags(getNativeRef(null));
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
        return 0;
    }

    private final native int N_GetXCoord(long self);
    private final native int N_GetYCoord(long self);
    private final native int N_GetTypeFlags(long self);
    private final native String N_GetLinkUrl(long self);
    private final native String N_GetUnfilteredLinkUrl(long self);
    private final native String N_GetSourceUrl(long self);
    private final native boolean N_HasImageContents(long self);
    private final native String N_GetPageUrl(long self);
    private final native String N_GetFrameUrl(long self);
    private final native String N_GetFrameCharset(long self);
    private final native MediaType N_GetMediaType(long self);
    private final native int N_GetMediaStateFlags(long self);
    private final native String N_GetSelectionText(long self);
    private final native String N_GetMisspelledWord(long self);
    private final native boolean N_GetDictionarySuggestions(long self, Vector<String> suggestions);
    private final native boolean N_IsEditable(long self);
    private final native boolean N_IsSpellCheckEnabled(long self);
    private final native int N_GetEditStateFlags(long self);
}
