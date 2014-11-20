// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.callback;

/**
 * Generic callback interface used for asynchronous completion.
 */
public interface CefCompletionCallback {

  /**
   * Method that will be called once the task is complete.
   */
  public abstract void onComplete();
}
