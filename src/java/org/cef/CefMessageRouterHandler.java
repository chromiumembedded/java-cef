// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef;

/**
 * Implement this interface to handle queries. All methods will be executed on
 * the browser process UI thread.
 */
public interface CefMessageRouterHandler {

  /**
   * Called when the browser receives a JavaScript query.
   * @param client The client generating the event.
   * @param query_id The unique ID for the query.
   * @param persistent True if the query is persistent.
   * @param callback Object used to continue or cancel the query asynchronously.
   */
  public void onQuery(CefClient client,
                      long query_id,
                      String request,
                      boolean persistent,
                      CefQueryCallback callback);

  /**
   * Called when a pending JavaScript query is canceled.
   * @param client The client generating the event.
   * @param query_id The unique ID for the query.
   */
  public void onQueryCanceled(CefClient client,
                              long query_id);
}
