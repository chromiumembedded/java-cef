// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef;

/**
 * An abstract adapter class for receiving message router events. 
 * The methods in this class are empty.
 * This class exists as convenience for creating handler objects.
 */
public abstract class CefMessageRouterHandlerAdapter implements CefMessageRouterHandler {

  @Override
  public void onQuery(CefClient client, 
                      long query_id, 
                      String request,
                      boolean persistent,
                      CefQueryCallback callback) {
    return;
  }

  @Override
  public void onQueryCanceled(CefClient client,
                              long query_id) {
    return;
  }
}
