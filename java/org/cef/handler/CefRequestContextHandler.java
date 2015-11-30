// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.handler;

import org.cef.network.CefCookieManager;
import org.cef.network.CefWebPluginInfo;

/**
 * Implement this interface to provide handler implementations.
 */
public interface CefRequestContextHandler {

  /**
   * Called on the IO thread to retrieve the cookie manager. The global cookie
   * manager will be used if this method returns NULL.
   */
  CefCookieManager getCookieManager();

  /**
   * Called on multiple browser process threads before a plugin instance is
   * loaded.
   * @param mime_type is the mime type of the plugin that will be loaded.
   * @param plugin_url is the content URL that the plugin will load and may be
   *        empty.
   * @param top_origin_url is the URL for the top-level frame that contains the
   *        plugin when loading a specific plugin instance or empty when
   *        building the initial list of enabled plugins for 'navigator.plugins'
   *        JavaScript state.
   * @param plugin_info includes additional information about the plugin that
   *        will be loaded. |plugin_policy| is the recommended policy.
   * @return true to block loading of the plugin.
   */
  boolean onBeforePluginLoad(String mime_type,
                             String plugin_url,
                             String top_origin_url,
                             CefWebPluginInfo plugin_info);
}
