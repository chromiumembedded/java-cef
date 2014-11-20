// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.network;

import java.util.Map;
import org.cef.callback.CefNative;

class CefResponse_N extends CefResponse implements CefNative {
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

  CefResponse_N() {
    super();
  }

  public static final CefResponse createNative() {
    CefResponse_N result = new CefResponse_N();
    try {
      result.N_CefResponse_CTOR();
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
      N_CefResponse_DTOR();
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
  public int getStatus() {
    try {
      return N_GetStatus();
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return 0;
  }

  @Override
  public void setStatus(int status) {
   try {
     N_SetStatus(status);
   } catch (UnsatisfiedLinkError ule) {
     ule.printStackTrace();
   }
  }

  @Override
  public String getStatusText() {
    try {
      return N_GetStatusText();
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return null;
  }

  @Override
  public void setStatusText(String statusText) {
    try {
      N_SetStatusText(statusText);
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
  }

  @Override
  public String getMimeType() {
    try {
      return N_GetMimeType();
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return null;
  }

  @Override
  public void setMimeType(String mimeType) {
    try {
      N_SetMimeType(mimeType);
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
  }

  @Override
  public String getHeader(String name) {
    try {
      return N_GetHeader(name);
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return null;
  }

  @Override
  public void getHeaderMap(Map<String, String> headerMap) {
    try {
      N_GetHeaderMap(headerMap);
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
  }

  @Override
  public void setHeaderMap(Map<String, String> headerMap) {
    try {
      N_SetHeaderMap(headerMap);
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
  }

  private final native void N_CefResponse_CTOR();
  private final native boolean N_IsReadOnly();
  private final native int N_GetStatus();
  private final native void N_SetStatus(int status);
  private final native String N_GetStatusText();
  private final native void N_SetStatusText(String statusText);
  private final native String N_GetMimeType();
  private final native void N_SetMimeType(String mimeType);
  private final native String N_GetHeader(String name);
  private final native void N_GetHeaderMap(Map<String,String> headerMap);
  private final native void N_SetHeaderMap(Map<String,String> headerMap);
  private final native void N_CefResponse_DTOR();
}
