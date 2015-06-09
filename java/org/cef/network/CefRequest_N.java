// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.network;

import java.util.Map;
import org.cef.callback.CefNative;

class CefRequest_N extends CefRequest implements CefNative {
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

  CefRequest_N() {
    super();
  }

  public static final CefRequest createNative() {
    CefRequest_N result = new CefRequest_N();
    try {
      result.N_CefRequest_CTOR();
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
      N_CefRequest_DTOR();
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
  public String getURL() {
    try {
      return N_GetURL();
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return null;
  }

  @Override
  public void setURL(String url) {
    try {
      N_SetURL(url);
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
  }

  @Override
  public String getMethod() {
    try {
      return N_GetMethod();
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return null;
  }

  @Override
  public void setMethod(String string) {
    try {
      N_SetMethod(string);
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
  }

  @Override
  public CefPostData getPostData() {
    try {
      return N_GetPostData();
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return null;
  }

  @Override
  public void setPostData(CefPostData postData) {
    try {
      N_SetPostData(postData);
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
  }

  @Override
  public void getHeaderMap(Map<String,String> headerMap) {
    try {
      N_GetHeaderMap(headerMap);
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
  }

  @Override
  public void setHeaderMap(Map<String,String> headerMap) {
    try {
      N_SetHeaderMap(headerMap);
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
  }

  @Override
  public void set(String url,
           String method,
           CefPostData postData,
           Map<String,String> headerMap) {
    try {
      N_Set(url, method, postData, headerMap);
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
  }

  @Override
  public int getFlags() {
    try {
      return N_GetFlags();
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return 0;
  }

  @Override
  public void setFlags(int flags) {
    try {
      N_SetFlags(flags);
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
  }

  @Override
  public String getFirstPartyForCookies() {
    try {
      return N_GetFirstPartyForCookies();
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return null;
  }

  @Override
  public void setFirstPartyForCookies(String url) {
    try {
      N_SetFirstPartyForCookies(url);
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
  }

  @Override
  public ResourceType getResourceType() {
    try {
      return N_GetResourceType();
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return ResourceType.RT_MAIN_FRAME;
  }

  @Override
  public TransitionType getTransitionType() {
    try {
      return N_GetTransitionType();
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return TransitionType.TT_AUTO_SUBFRAME;
  }

  private final native void N_CefRequest_CTOR();
  private final native boolean N_IsReadOnly();
  private final native String N_GetURL();
  private final native void N_SetURL(String url);
  private final native String N_GetMethod();
  private final native void N_SetMethod(String string);
  private final native CefPostData N_GetPostData();
  private final native void N_SetPostData(CefPostData postData);
  private final native void N_GetHeaderMap(Map<String,String> headerMap);
  private final native void N_SetHeaderMap(Map<String,String> headerMap);
  private final native void N_Set(String url,
                                  String method,
                                  CefPostData postData,
                                  Map<String,String> headerMap);
  private final native int N_GetFlags();
  private final native void N_SetFlags(int flags);
  private final native String N_GetFirstPartyForCookies();
  private final native void N_SetFirstPartyForCookies(String url);
  private final native ResourceType N_GetResourceType();
  private final native TransitionType N_GetTransitionType();
  private final native void N_CefRequest_DTOR();
}
