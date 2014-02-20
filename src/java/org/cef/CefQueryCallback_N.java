// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef;

class CefQueryCallback_N implements CefQueryCallback {
  // Used internally to store a pointer to the CEF object.
  private long N_CefHandle = 0;

  // Constructor is called by native code.
  CefQueryCallback_N() {
  }

  @Override
  public void success(String response) {
    try {
      N_Success(response);
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
  }

  @Override
  public void failure(int error_code, String error_message) {
    try {
      N_Failure(error_code, error_message);
    } catch (UnsatisfiedLinkError ule) {
      ule.printStackTrace();
    }
  }

  private final native void N_Success(String response);
  private final native void N_Failure(int error_code, String error_message);
}
