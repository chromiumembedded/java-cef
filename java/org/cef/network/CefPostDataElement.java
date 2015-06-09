// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.network;

/**
 * Class used to represent a single element in the request post data. The
 * methods of this class may be called on any thread.
 */
public abstract class CefPostDataElement {

  /**
   * Post data elements may represent either bytes or files.
   */
  public static enum Type {
    PDE_TYPE_EMPTY,
    PDE_TYPE_BYTES,
    PDE_TYPE_FILE,
  }

  //This CTOR can't be called directly. Call method create() instead.
  CefPostDataElement() {
  }

  /**
   * Create a new CefPostDataElement object.
   */
  public static final CefPostDataElement create() {
    return CefPostDataElement_N.createNative();
  }

  /**
   * Returns true if this object is read-only.
   */
  public abstract boolean isReadOnly();

  /**
   * Remove all contents from the post data element.
   */
  public abstract void setToEmpty();

  /**
   * The post data element will represent a file.
   */
  public abstract void setToFile(String fileName);

  /**
   * The post data element will represent bytes.  The bytes passed
   * in will be copied.
   */
  public abstract void setToBytes(int size, byte[] bytes);

  /**
   * Return the type of this post data element.
   */
  public abstract Type getType();

  /**
   * Return the file name.
   */
  public abstract String getFile();

  /**
   * Return the number of bytes.
   */
  public abstract int getBytesCount();

  /**
   * Read up to size bytes into bytes and return the number of bytes
   * actually read.
   */
  public abstract int getBytes(int size, byte[] bytes);

  @Override
  public String toString() {
    return toString(null);
  }

  public String toString(String mimeType) {
    int bytesCnt = getBytesCount();
    byte[] bytes = null;
    if (bytesCnt > 0) {
      bytes = new byte[bytesCnt];
    }

    boolean asText = false;
    if (mimeType != null) {
      if (mimeType.startsWith("text/"))
        asText = true;
      else if (mimeType.startsWith("application/xml"))
        asText = true;
      else if (mimeType.startsWith("application/xhtml"))
        asText = true;
      else if (mimeType.startsWith("application/x-www-form-urlencoded"))
        asText = true;
    }

    String returnValue = "";

    if (getType() == Type.PDE_TYPE_BYTES) {
      int setBytes = getBytes(bytes.length, bytes);
      returnValue += "    Content-Length: " + bytesCnt + "\n";
      if (asText) {
        returnValue += "\n    " + new String(bytes);
      } else {
        for (int i=0; i < setBytes; i++) {
          if (i%40 == 0)
            returnValue += "\n    ";
          returnValue += String.format("%02X", bytes[i]) + " ";
        }
      }
      returnValue += "\n";
    } else if (getType() == Type.PDE_TYPE_FILE) {
      returnValue += "\n    Bytes of file: " + getFile() + "\n";
    }
    return returnValue;
  }
}
