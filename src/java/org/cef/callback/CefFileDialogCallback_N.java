// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.callback;

import java.util.Vector;

class CefFileDialogCallback_N extends CefNativeAdapter implements
    CefFileDialogCallback {

  CefFileDialogCallback_N() {
  }

  @Override
  public void Continue(Vector<String> filePaths) {
    try {
      N_Continue(filePaths);
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

  private final native void N_Continue(Vector<String> filePaths);
  private final native void N_Cancel();
}
