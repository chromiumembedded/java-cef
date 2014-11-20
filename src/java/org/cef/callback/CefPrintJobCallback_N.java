// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.callback;

class CefPrintJobCallback_N extends CefNativeAdapter
    implements CefPrintJobCallback {

  CefPrintJobCallback_N() {
  }

  @Override
  public void Continue() {
    try {
      N_Continue();
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
  }

  private final native void N_Continue();
}
