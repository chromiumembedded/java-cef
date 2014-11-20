// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.callback;

class CefJSDialogCallback_N extends CefNativeAdapter implements
    CefJSDialogCallback {

  CefJSDialogCallback_N() {
  }

  @Override
  public void Continue(boolean success, String user_input) {
    try {
      N_Continue(success,user_input);
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
  }

  private final native void N_Continue(boolean success, String user_input);
}
