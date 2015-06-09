// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package tests.detailed.handler;

import java.awt.Frame;

import javax.swing.JOptionPane;
import javax.swing.SwingUtilities;

import org.cef.browser.CefBrowser;
import org.cef.callback.CefGeolocationCallback;
import org.cef.handler.CefGeolocationHandlerAdapter;

public class GeolocationHandler extends CefGeolocationHandlerAdapter {
  private final Frame owner_;
  public GeolocationHandler(Frame owner) {
    owner_ = owner;
  }

  @Override
  public boolean onRequestGeolocationPermission(CefBrowser browser,
                                             String requesting_url,
                                             int request_id,
                                             CefGeolocationCallback callback) {
    final CefGeolocationCallback cb = callback;
    final String reqUrl = requesting_url;
    SwingUtilities.invokeLater(new Runnable() {
      @Override
      public void run() {
        int dialogResult = JOptionPane.showConfirmDialog(owner_,
            "The URL \n" + reqUrl +
            "\nwants to request your geolocation." +
            "\nDo you want to proceed?",
            "Geolocation requested",
            JOptionPane.YES_NO_OPTION,
            JOptionPane.QUESTION_MESSAGE);
        cb.Continue(dialogResult == JOptionPane.YES_OPTION);
      }
    });
    return true;
  }
}
