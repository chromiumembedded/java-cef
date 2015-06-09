// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.callback;

import java.util.Vector;

class CefContextMenuParams_N extends CefNativeAdapter implements
    CefContextMenuParams {

  CefContextMenuParams_N() {
  }

  @Override
  public int getXCoord() {
    try {
      return N_GetXCoord();
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return 0;
  }

  @Override
  public int getYCoord() {
    try {
      return N_GetYCoord();
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return 0;
  }

  @Override
  public int getTypeFlags() {
    try {
      return N_GetTypeFlags();
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return 0;
  }

  @Override
  public String getLinkUrl() {
    try {
      return N_GetLinkUrl();
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return null;
  }

  @Override
  public String getUnfilteredLinkUrl() {
    try {
      return N_GetUnfilteredLinkUrl();
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return null;
  }

  @Override
  public String getSourceUrl() {
    try {
      return N_GetSourceUrl();
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return null;
  }

  @Override
  public boolean hasImageContents() {
    try {
      return N_HasImageContents();
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return false;
  }

  @Override
  public String getPageUrl() {
    try {
      return N_GetPageUrl();
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return null;
  }

  @Override
  public String getFrameUrl() {
    try {
      return N_GetFrameUrl();
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return null;
  }

  @Override
  public String getFrameCharset() {
    try {
      return N_GetFrameCharset();
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return null;
  }

  @Override
  public MediaType getMediaType() {
    try {
      return N_GetMediaType();
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return null;
  }

  @Override
  public int getMediaStateFlags() {
    try {
      return N_GetMediaStateFlags();
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return 0;
  }

  @Override
  public String getSelectionText() {
    try {
      return N_GetSelectionText();
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return null;
  }

  @Override
  public String getMisspelledWord() {
    try {
      return N_GetMisspelledWord();
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return null;
  }

  @Override
  public boolean getDictionarySuggestions(Vector<String> suggestions) {
    try {
      return N_GetDictionarySuggestions(suggestions);
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return false;
  }

  @Override
  public boolean isEditable() {
    try {
      return N_IsEditable();
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return false;
  }

  @Override
  public boolean isSpellCheckEnabled() {
    try {
      return N_IsSpellCheckEnabled();
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return false;
  }

  @Override
  public int getEditStateFlags() {
    try {
      return N_GetEditStateFlags();
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return 0;
  }

  private final native int N_GetXCoord();
  private final native int N_GetYCoord();
  private final native int N_GetTypeFlags();
  private final native String N_GetLinkUrl();
  private final native String N_GetUnfilteredLinkUrl();
  private final native String N_GetSourceUrl();
  private final native boolean N_HasImageContents();
  private final native String N_GetPageUrl();
  private final native String N_GetFrameUrl();
  private final native String N_GetFrameCharset();
  private final native MediaType N_GetMediaType();
  private final native int N_GetMediaStateFlags();
  private final native String N_GetSelectionText();
  private final native String N_GetMisspelledWord();
  private final native boolean N_GetDictionarySuggestions(
      Vector<String> suggestions);
  private final native boolean N_IsEditable();
  private final native boolean N_IsSpellCheckEnabled();
  private final native int N_GetEditStateFlags();
}
