// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.callback;

import java.util.Vector;

/**
 * Class used to represent drag data. The methods of this class may be called
 * on any thread.
 */
public interface CefDragData extends CefNative {
  /**
   * Returns true if the drag data is a link.
   */
  boolean isLink();

  /**
   * Returns true if the drag data is a text or html fragment.
   */
  boolean isFragment();

  /**
   * Returns true if the drag data is a file.
   */
  boolean isFile();

  /**
   * Return the link URL that is being dragged.
   */
  String getLinkURL();

  /**
   * Return the title associated with the link being dragged.
   */
  String getLinkTitle();

  /**
   * Return the metadata, if any, associated with the link being dragged.
   */
  String getLinkMetadata();

  /**
   * Return the plain text fragment that is being dragged.
   */
  String getFragmentText();

  /**
   * Return the text/html fragment that is being dragged.
   */
  String getFragmentHtml();

  /**
   * Return the base URL that the fragment came from. This value is used for
   * resolving relative URLs and may be empty.
   */
  String getFragmentBaseURL();

  /**
   * Return the name of the file being dragged out of the browser window.
   */
  String getFileName();

  /**
   * Retrieve the list of file names that are being dragged into the browser
   * window.
   */
  boolean getFileNames(Vector<String> names);
}
