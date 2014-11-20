// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.callback;

class CefAuthCallback_N extends CefNativeAdapter implements CefAuthCallback {

  CefAuthCallback_N() {
  }

  @Override
  public void Continue(String username, String password) {
    try {
      N_Continue(username, password);
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
  }

  @Override
  public void cancel() {
    try {
      N_Cancel();
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
  }

  private final native void N_Continue(String username, String password);
  private final native void N_Cancel();
}
