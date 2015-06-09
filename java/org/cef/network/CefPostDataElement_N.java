// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.network;

import org.cef.callback.CefNative;

class CefPostDataElement_N extends CefPostDataElement implements CefNative {
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

  CefPostDataElement_N() {
    super();
  }

  public static final CefPostDataElement createNative() {
    CefPostDataElement_N result = new CefPostDataElement_N();
    try {
      result.N_CefPostDataElement_CTOR();
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
      N_CefPostDataElement_DTOR();
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
  public void setToEmpty() {
    try {
      N_SetToEmpty();
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
  }

  @Override
  public void setToFile(String fileName) {
    try {
      N_SetToFile(fileName);
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
  }

  @Override
  public void setToBytes(int size, byte[] bytes) {
    try {
      N_SetToBytes(size, bytes);
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
  }

  @Override
  public Type getType() {
    try {
      return N_GetType();
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return null;
  }

  @Override
  public String getFile() {
    try {
      return N_GetFile();
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return null;
  }

  @Override
  public int getBytesCount() {
    try {
      return N_GetBytesCount();
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return 0;
  }

  @Override
  public int getBytes(int size, byte[] bytes) {
    try {
      return N_GetBytes(size, bytes);
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return 0;
  }

  private final native void N_CefPostDataElement_CTOR();
  private final native boolean N_IsReadOnly();
  private final native void N_SetToEmpty();
  private final native void N_SetToFile(String fileName);
  private final native void N_SetToBytes(int size, byte[] bytes);
  private final native Type N_GetType();
  private final native String N_GetFile();
  private final native int N_GetBytesCount();
  private final native int N_GetBytes(int size, byte[] bytes);
  private final native void N_CefPostDataElement_DTOR();
}
