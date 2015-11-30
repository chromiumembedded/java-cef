// Copyright (c) 2015 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.handler;

import org.cef.network.CefCookieManager;
import org.cef.network.CefWebPluginInfo;

/**
 * An abstract adapter class for receiving browser request context events.
 * The methods in this class are empty.
 * This class exists as convenience for creating handler objects.
 */
public abstract class CefRequestContextHandlerAdapter
    implements CefRequestContextHandler {

  @Override
  public CefCookieManager getCookieManager() {
    return null;
  }

  @Override
  public boolean onBeforePluginLoad(String mime_type,
                                    String plugin_url,
                                    String top_origin_url,
                                    CefWebPluginInfo plugin_info) {
    return false;
  }
}
