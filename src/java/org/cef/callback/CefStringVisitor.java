// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.callback;

/**
 * Public interface to receive string values asynchronously.
 */
public interface CefStringVisitor {
  /**
   * Called when the string is available.
   * @param string Requested string.
   */
  void visit(String string);
}
