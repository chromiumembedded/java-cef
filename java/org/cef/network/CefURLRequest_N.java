// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.network;

import org.cef.callback.CefURLRequestClient;
import org.cef.callback.CefNative;
import org.cef.handler.CefLoadHandler.ErrorCode;

class CefURLRequest_N extends CefURLRequest implements CefNative {
  // Used internally to store a pointer to the CEF object.
  private long N_CefHandle = 0;
  private final CefRequest request_;
  private final CefURLRequestClient client_;

  @Override
  public void setNativeRef(String identifer, long nativeRef) {
    N_CefHandle = nativeRef;
  }

  @Override
  public long getNativeRef(String identifer) {
    return N_CefHandle;
  }

  CefURLRequest_N(CefRequest request,
                  CefURLRequestClient client) {
    super();
    request_ = request;
    client_ = client;
  }

  public static final CefURLRequest createNative(CefRequest request,
                                                 CefURLRequestClient client) {
    // keep a reference to the request and client objects.
    CefURLRequest_N result = new CefURLRequest_N(request, client);
    try {
      result.N_CefURLRequest_CTOR(request, client);
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    if (result.N_CefHandle == 0)
      return null;
    return result;
  }

  @Override
  public void finalize() {
    try {
      N_CefURLRequest_DTOR();
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
  }

  @Override
  public CefRequest getRequest() {
    return request_;
  }

  @Override
  public CefURLRequestClient getClient() {
    return client_;
  }

  @Override
  public Status getRequestStatus() {
    try {
      return N_GetRequestStatus();
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return null;
  }

  @Override
  public ErrorCode getRequestError() {
    try {
      return N_GetRequestError();
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return null;
  }

  @Override
  public CefResponse getResponse() {
    try {
      return N_GetResponse();
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
    return null;
  }

  @Override
  public void cancel() {
    try {
      N_Cancel();
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
  }

  private final native void N_CefURLRequest_CTOR(CefRequest request, CefURLRequestClient client);
  private final native Status N_GetRequestStatus();
  private final native ErrorCode N_GetRequestError();
  private final native CefResponse N_GetResponse();
  private final native void N_Cancel();
  private final native void N_CefURLRequest_DTOR();
}
