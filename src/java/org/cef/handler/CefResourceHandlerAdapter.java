// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.handler;

import org.cef.callback.CefCallback;
import org.cef.callback.CefNativeAdapter;
import org.cef.misc.IntRef;
import org.cef.misc.StringRef;
import org.cef.network.CefCookie;
import org.cef.network.CefRequest;
import org.cef.network.CefResponse;

/**
 * An abstract adapter class for receiving resource requests.
 * The methods in this class are empty.
 * This class exists as convenience for creating handler objects.
 */
public abstract class CefResourceHandlerAdapter extends CefNativeAdapter
    implements CefResourceHandler {

  @Override
  public boolean processRequest(CefRequest request,
                                CefCallback callback) {
    return false;
  }

  @Override
  public void getResponseHeaders(CefResponse response,
                                 IntRef response_length,
                                 StringRef redirectUrl) {
  }

  @Override
  public boolean readResponse(byte[] data_out,
                              int bytes_to_read,
                              IntRef bytes_read,
                              CefCallback callback) {
    return false;
  }

  @Override
  public boolean canGetCookie(CefCookie cookie) {
    return true;
  }

  @Override
  public boolean canSetCookie(CefCookie cookie) {
    return true;
  }

  @Override
  public void cancel() {
  }
}
