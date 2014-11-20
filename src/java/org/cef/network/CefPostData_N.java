// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.network;

import java.util.Vector;
import org.cef.callback.CefNative;

/**
 *
 */
class CefPostData_N extends CefPostData implements CefNative {
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

  CefPostData_N() {
    super();
  }

  public static final CefPostData createNative() {
    CefPostData_N result = new CefPostData_N();
    try {
      result.N_CefPostData_CTOR();
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    if (result.N_CefHandle == 0)
      return null;
    return result;
  }

  @Override
  protected void finalize() throws Throwable {
    try {
      N_CefPostData_DTOR();
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    } finally {
      super.finalize();
    }
  }

  @Override
  public boolean isReadOnly() {
    try {
      return N_IsReadOnly();
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return false;
  }

  @Override
  public int getElementCount() {
    try {
      return N_GetElementCount();
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return 0;
  }

  @Override
  public void getElements(Vector<CefPostDataElement> elements) {
    try {
      N_GetElements(elements);
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
  }

  @Override
  public boolean removeElement(CefPostDataElement element) {
    try {
      return N_RemoveElement(element);
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return false;
  }

  @Override
  public boolean addElement(CefPostDataElement element) {
    try {
      return N_AddElement(element);
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return false;
  }

  @Override
  public void removeElements() {
    try {
      N_RemoveElements();
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
  }

  private final native void N_CefPostData_CTOR();
  private final native boolean N_IsReadOnly();
  private final native int N_GetElementCount();
  private final native void N_GetElements(Vector<CefPostDataElement> elements);
  private final native boolean N_RemoveElement(CefPostDataElement element);
  private final native boolean N_AddElement(CefPostDataElement element);
  private final native void N_RemoveElements();
  private final native void N_CefPostData_DTOR();
}
