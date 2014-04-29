// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package tests.detailed.handler;

import java.awt.Frame;

import javax.swing.SwingUtilities;

import org.cef.browser.CefBrowser;
import org.cef.callback.CefAllowCertificateErrorCallback;
import org.cef.callback.CefAuthCallback;
import org.cef.handler.CefRequestHandlerAdapter;
import org.cef.handler.CefLoadHandler.ErrorCode;

import tests.detailed.dialog.CertErrorDialog;
import tests.detailed.dialog.PasswordDialog;

public class RequestHandler extends CefRequestHandlerAdapter {
  private final Frame owner_;

  public RequestHandler(Frame owner) {
    owner_ = owner;
  }

  @Override
  public boolean getAuthCredentials(CefBrowser browser,
                                    boolean isProxy,
                                    String host,
                                    int port,
                                    String realm,
                                    String scheme,
                                    CefAuthCallback callback) {
    SwingUtilities.invokeLater(new PasswordDialog(owner_, callback));
    return true;
  }

  @Override
  public boolean onCertificateError(ErrorCode cert_error,
                                    String request_url,
                                    CefAllowCertificateErrorCallback callback) {
    SwingUtilities.invokeLater(new CertErrorDialog(owner_, cert_error, request_url, callback));
    return true;
  }
}
