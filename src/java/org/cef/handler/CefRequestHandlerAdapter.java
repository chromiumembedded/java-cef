// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.handler;

import org.cef.browser.CefBrowser;
import org.cef.callback.CefAllowCertificateErrorCallback;
import org.cef.callback.CefAuthCallback;
import org.cef.callback.CefQuotaCallback;
import org.cef.handler.CefLoadHandler.ErrorCode;
import org.cef.misc.BoolRef;
import org.cef.misc.StringRef;
import org.cef.network.CefRequest;
import org.cef.network.CefWebPluginInfo;

/**
 * An abstract adapter class for receiving browser request events.
 * The methods in this class are empty.
 * This class exists as convenience for creating handler objects.
 */
public abstract class CefRequestHandlerAdapter implements CefRequestHandler {
  @Override
  public boolean onBeforeBrowse(CefBrowser browser,
                                CefRequest request,
                                boolean is_redirect) {
    return false;
  }

  @Override
  public boolean onBeforeResourceLoad(CefBrowser browser, CefRequest request) {
    return false;
  }

  @Override
  public CefResourceHandler getResourceHandler(CefBrowser browser,
                                               CefRequest request) {
    return null;
  }

  @Override
  public void onResourceRedirect(CefBrowser browser,
                                 String old_url,
                                 StringRef new_url) {
  }

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
  public void onProtocolExecution(CefBrowser browser,
                                  String url,
                                  BoolRef allow_os_execution) {
  }

  @Override
  public boolean onCertificateError(ErrorCode cert_error,
                                    String request_url,
                                    CefAllowCertificateErrorCallback callback) {
    return false;
  }

  @Override
  public boolean onBeforePluginLoad(CefBrowser browser,
                                    String url,
                                    String policyUrl,
                                    CefWebPluginInfo info) {
    return false;
  }

  @Override
  public void onPluginCrashed(CefBrowser browser,
                              String pluginPath) {
  }

  @Override
  public void onRenderProcessTerminated(CefBrowser browser,
                                        TerminationStatus status) {
  }
}
