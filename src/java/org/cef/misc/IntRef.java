// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.misc;

/**
 * Helper class for passing int values by reference.
 */
public class IntRef {
  private int value_;

  public IntRef() {
  }

  public IntRef(int value) {
    value_ = value;
  }

  public void set(int value) {
    value_ = value;
  }

  public int get() {
    return value_;
  }
}
