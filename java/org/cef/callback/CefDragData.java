// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.callback;

import java.io.OutputStream;
import java.util.Vector;

/**
 * Class used to represent drag data. The methods of this class may be called
 * on any thread.
 */
public abstract class CefDragData {

  //This CTOR can't be called directly. Call method create() instead.
  CefDragData() {
  }

  @Override
  protected void finalize() throws Throwable {
    dispose();
    super.finalize();
  }

  /**
   * Create a new CefDragData object.
   */
  public static final CefDragData create() {
    return CefDragData_N.createNative();
  }

  /**
   * Returns a copy of the current object
   */
  public abstract CefDragData clone();

  /**
   * Removes the native reference from an unused object.
   */
  public abstract void dispose();

  /**
   * Test if the object is set to read-only.
   * @return true if this object is read-only.
   */
  public abstract boolean isReadOnly();

  /**
   * Returns true if the drag data is a link.
   */
  public abstract boolean isLink();

  /**
   * Returns true if the drag data is a text or html fragment.
   */
  public abstract boolean isFragment();

  /**
   * Returns true if the drag data is a file.
   */
  public abstract boolean isFile();

  /**
   * Return the link URL that is being dragged.
   */
  public abstract String getLinkURL();

  /**
   * Return the title associated with the link being dragged.
   */
  public abstract String getLinkTitle();

  /**
   * Return the metadata, if any, associated with the link being dragged.
   */
  public abstract String getLinkMetadata();

  /**
   * Return the plain text fragment that is being dragged.
   */
  public abstract String getFragmentText();

  /**
   * Return the text/html fragment that is being dragged.
   */
  public abstract String getFragmentHtml();

  /**
   * Return the base URL that the fragment came from. This value is used for
   * resolving relative URLs and may be empty.
   */
  public abstract String getFragmentBaseURL();

  /**
   * Write the contents of the file being dragged out of the web view into
   * |writer|. Returns the number of bytes sent to |writer|. If |writer| is
   * NULL this method will return the size of the file contents in bytes.
   * Call getFileName() to get a suggested name for the file.
   *
   * @param writer Writes the contents into this object.
   * @return The number of bytes sent to writer. If writer is NULL the size of
   *   the file contents in bytes is returned.
   */
  public abstract int getFileContents(OutputStream writer);

  /**
   * Return the name of the file being dragged out of the browser window.
   */
  public abstract String getFileName();

  /**
   * Retrieve the list of file names that are being dragged into the browser
   * window.
   */
  public abstract boolean getFileNames(Vector<String> names);

  /**
   * Set the link URL that is being dragged.
   * @param url The link URL to be set.
   */
  public abstract void setLinkURL(String url);

  /**
   * Set the title associated with the link being dragged.
   * @param title The tile associated with the link.
   */
  public abstract void setLinkTitle(String title);

  /**
   * Set the metadata associated with the link being dragged.
   * @param data The metadata associated with the link.
   */
  public abstract void setLinkMetadata(String data);

  /**
   * Set the plain text fragment that is being dragged.
   * @param text The plain text fragment to be set.
   */
  public abstract void setFragmentText(String text);

  /**
   * Set the text/html fragment that is being dragged.
   * @param html The html fragment to be set.
   */
  public abstract void setFragmentHtml(String html);

  /**
   * Set the base URL that the fragment came from.
   * @param baseUrl The base URL to be set.
   */
  public abstract void setFragmentBaseURL(String baseUrl);

  /**
   * Reset the file contents. You should do this before calling
   * CefBrowser.dragTargetDragEnter as the web view does not allow us to
   * drag in this kind of data.
   */
  public abstract void resetFileContents();

  /**
   * Add a file that is being dragged into the webview.
   * @param path The file and path to be set.
   * @param displayName The name to be displayed.
   */
  public abstract void addFile(String path, String displayName);

  @Override
  public String toString() {
    return "CefDragData [isReadOnly()=" + isReadOnly() + ", isLink()="
        + isLink() + ", isFragment()=" + isFragment() + ", isFile()="
        + isFile() + ", getLinkURL()=" + getLinkURL() + ", getLinkTitle()="
        + getLinkTitle() + ", getLinkMetadata()=" + getLinkMetadata()
        + ", getFragmentText()=" + getFragmentText() + ", getFragmentHtml()="
        + getFragmentHtml() + ", getFragmentBaseURL()=" + getFragmentBaseURL()
        + ", getFileName()=" + getFileName() + "]";
  }
}
