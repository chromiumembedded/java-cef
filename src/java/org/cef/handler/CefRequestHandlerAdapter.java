// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.handler;

import org.cef.browser.CefBrowser;
import org.cef.callback.CefAllowCertificateErrorCallback;
import org.cef.callback.CefAuthCallback;
import org.cef.callback.CefQuotaCallback;
import org.cef.handler.CefLoadHandler.ErrorCode;

/**
 * An abstract adapter class for receiving browser request events.
 * The methods in this class are empty.
 * This class exists as convenience for creating handler objects.
 */
public abstract class CefRequestHandlerAdapter implements CefRequestHandler {
  @Override
  public boolean getAuthCredentials(CefBrowser browser,
                                    boolean isProxy,
                                    String host,
                                    int port,
                                    String realm,
                                    String scheme,
                                    CefAuthCallback callback) {
    return false;
  }

  @Override
  public boolean onQuotaRequest(CefBrowser browser,
                                String origin_url,
                                long new_size,
                                CefQuotaCallback callback) {
    return false;
  }

  @Override
  public boolean onCertificateError(ErrorCode cert_error,
                                    String request_url,
                                    CefAllowCertificateErrorCallback callback) {
    return false;
  }
}
