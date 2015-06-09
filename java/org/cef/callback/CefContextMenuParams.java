// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.callback;

import java.util.Vector;

/**
 *  Provides information about the context menu state. The methods of this class
 * can only be accessed on browser process the UI thread.
 */
public interface CefContextMenuParams {

  /**
   * Supported context menu type flags.
   */
  public static final class TypeFlags {
    public final static int CM_TYPEFLAG_NONE        = 0;      //!< No node is selected.
    public final static int CM_TYPEFLAG_PAGE        = 1 << 0; //!< The top page is selected.
    public final static int CM_TYPEFLAG_FRAME       = 1 << 1; //!< A subframe page is selected.
    public final static int CM_TYPEFLAG_LINK        = 1 << 2; //!< A link is selected.
    public final static int CM_TYPEFLAG_MEDIA       = 1 << 3; //!< A media node is selected.
    public final static int CM_TYPEFLAG_SELECTION   = 1 << 4; //!< There is a textual or mixed selection that is selected.
    public final static int CM_TYPEFLAG_EDITABLE    = 1 << 5; //!< An editable element is selected.
  }

  /**
   * Supported context menu media types.
   */
  public enum MediaType {
    CM_MEDIATYPE_NONE,    //!< No special node is in context.
    CM_MEDIATYPE_IMAGE,   //!< An image node is selected.
    CM_MEDIATYPE_VIDEO,   //!< A video node is selected.
    CM_MEDIATYPE_AUDIO,   //!< An audio node is selected.
    CM_MEDIATYPE_FILE,    //!< A file node is selected.
    CM_MEDIATYPE_PLUGIN,  //!< A plugin node is selected.
  }

  /**
   * Supported context menu media state bit flags.
   */
  public static final class MediaStateFlags {
    public final static int CM_MEDIAFLAG_NONE                  = 0;
    public final static int CM_MEDIAFLAG_ERROR                 = 1 << 0;
    public final static int CM_MEDIAFLAG_PAUSED                = 1 << 1;
    public final static int CM_MEDIAFLAG_MUTED                 = 1 << 2;
    public final static int CM_MEDIAFLAG_LOOP                  = 1 << 3;
    public final static int CM_MEDIAFLAG_CAN_SAVE              = 1 << 4;
    public final static int CM_MEDIAFLAG_HAS_AUDIO             = 1 << 5;
    public final static int CM_MEDIAFLAG_HAS_VIDEO             = 1 << 6;
    public final static int CM_MEDIAFLAG_CONTROL_ROOT_ELEMENT  = 1 << 7;
    public final static int CM_MEDIAFLAG_CAN_PRINT             = 1 << 8;
    public final static int CM_MEDIAFLAG_CAN_ROTATE            = 1 << 9;
  }

  /**
   * Supported context menu edit state bit flags.
   */
  public static final class EditStateFlags {
    public final static int CM_EDITFLAG_NONE            = 0;
    public final static int CM_EDITFLAG_CAN_UNDO        = 1 << 0;
    public final static int CM_EDITFLAG_CAN_REDO        = 1 << 1;
    public final static int CM_EDITFLAG_CAN_CUT         = 1 << 2;
    public final static int CM_EDITFLAG_CAN_COPY        = 1 << 3;
    public final static int CM_EDITFLAG_CAN_PASTE       = 1 << 4;
    public final static int CM_EDITFLAG_CAN_DELETE      = 1 << 5;
    public final static int CM_EDITFLAG_CAN_SELECT_ALL  = 1 << 6;
    public final static int CM_EDITFLAG_CAN_TRANSLATE   = 1 << 7;
  }

  /**
   * Returns the X coordinate of the mouse where the context menu was invoked.
   * Coords are relative to the associated RenderView's origin.
   */
  int getXCoord();

  /**
   * Returns the Y coordinate of the mouse where the context menu was invoked.
   * Coords are relative to the associated RenderView's origin.
   */
  int getYCoord();

  /**
   * Returns flags representing the type of node that the context menu was
   * invoked on. See TypeFlags for supported values
   */
  int getTypeFlags();

  /**
   * Returns the URL of the link, if any, that encloses the node that the
   * context menu was invoked on.
   */
  String getLinkUrl();

  /**
   * Returns the link URL, if any, to be used ONLY for "copy link address". We
   * don't validate this field in the frontend process.
   */
  String getUnfilteredLinkUrl();

  /**
   * Returns the source URL, if any, for the element that the context menu was
   * invoked on. Example of elements with source URLs are img, audio, and video.
   */
  String getSourceUrl();

  /**
   * Returns true if the context menu was invoked on an image which has
   * non-empty contents.
   */
  boolean hasImageContents();

  /**
   * Returns the URL of the top level page that the context menu was invoked on.
   */
  String getPageUrl();

  /**
   * Returns the URL of the subframe that the context menu was invoked on.
   */
  String getFrameUrl();

  /**
   * Returns the character encoding of the subframe that the context menu was
   * invoked on.
   */
  String getFrameCharset();

  /**
   * Returns the type of context node that the context menu was invoked on.
   */
  MediaType getMediaType();

  /**
   * Returns flags representing the actions supported by the media element, if
   * any, that the context menu was invoked on. See MediaStateFlags for possible
   * values.
   */
  int getMediaStateFlags();

  /**
   * Returns the text of the selection, if any, that the context menu was
   * invoked on.
   */
  
  String getSelectionText();

  /**
   * Returns the text of the misspelled word, if any, that the context menu was
   * invoked on.
   */
  String getMisspelledWord();

  /**
   * Returns true if suggestions exist, false otherwise. Fills in |suggestions|
   * from the spell check service for the misspelled word if there is one.
   */
  boolean getDictionarySuggestions(Vector<String> suggestions);

  /**
   * Returns true if the context menu was invoked on an editable node.
   */
  
  boolean isEditable();

  /**
   * Returns true if the context menu was invoked on an editable node where
   * spell-check is enabled.
   */
  
  boolean isSpellCheckEnabled();

  /**
   * Returns flags representing the actions supported by the editable node, if
   * any, that the context menu was invoked on. See EditStateFlags for possible
   * values.
   */
  int getEditStateFlags();
}
