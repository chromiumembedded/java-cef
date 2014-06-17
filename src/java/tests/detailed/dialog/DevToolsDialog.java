// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package tests.detailed.dialog;

import java.awt.BorderLayout;
import java.awt.Frame;
import java.awt.event.ComponentAdapter;
import java.awt.event.ComponentEvent;

import javax.swing.JDialog;

import org.cef.browser.CefBrowser;

@SuppressWarnings("serial")
public class DevToolsDialog extends JDialog {
  private final CefBrowser devTools_;
  public DevToolsDialog(Frame owner, String title, CefBrowser browser) {
    super(owner, title, false);

    setLayout(new BorderLayout());
    setSize(800, 600);
    setLocation(owner.getLocation().x+20, owner.getLocation().y+20);

    devTools_ = browser.getDevTools();
    add(devTools_.getUIComponent());

    addComponentListener(new ComponentAdapter() {
      @Override
      public void componentHidden(ComponentEvent e) {
        dispose();
      }
    });
  }

  @Override
  public void dispose() {
    devTools_.close();
    super.dispose();
  }
}
