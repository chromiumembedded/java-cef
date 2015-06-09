// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.misc;

/**
 * Helper class for passing boolean values by reference.
 */
public class BoolRef {
  private boolean value_;

  public BoolRef() {
  }

  public BoolRef(boolean value) {
    value_ = value;
  }

  public void set(boolean value) {
    value_ = value;
  }

  public boolean get() {
    return value_;
  }
}
