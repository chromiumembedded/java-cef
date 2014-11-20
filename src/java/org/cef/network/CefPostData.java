// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.network;

import java.util.Vector;

/**
 * Class used to represent post data for a web request. The methods of this
 * class may be called on any thread.
 */
public abstract class CefPostData {
  
  //This CTOR can't be called directly. Call method create() instead.
  CefPostData() {
  }

  /**
   * Create a new CefPostData object.
   */
  public static final CefPostData create() {
    return CefPostData_N.createNative();
  }

  /**
   * Returns true if this object is read-only.
   */
  public abstract boolean isReadOnly();

  /**
   * Returns the number of existing post data elements.
   */
  public abstract int getElementCount();

  /**
   * Retrieve the post data elements.
   */
  public abstract void getElements(Vector<CefPostDataElement> elements);

  /**
   * Remove the specified post data element. Returns true if the removal
   * succeeds.
   */
  public abstract boolean removeElement(CefPostDataElement element);

  /**
   * Add the specified post data element. Returns true if the add succeeds.
   */
  public abstract boolean addElement(CefPostDataElement element);

  /**
   * Remove all existing post data elements.
   */
  public abstract void removeElements();

  @Override
  public String toString() {
    return toString(null);
  }

  public String toString(String mimeType) {
    Vector<CefPostDataElement> elements = new Vector<CefPostDataElement>();
    getElements(elements);

    String returnValue = "";
    for (CefPostDataElement el : elements) {
      returnValue += el.toString(mimeType) + "\n";
    }
    return returnValue;
  }
}
