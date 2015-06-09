// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.network;

/**
 * Information about a specific web plugin.
 */
public interface CefWebPluginInfo {

  /**
   * Returns the plugin name (i.e. Flash).
   */
  public String getName();

  /**
   * Returns the plugin file path (DLL/bundle/library).
   */
  public String getPath();

  /**
   * Returns the version of the plugin (may be OS-specific).
   */
  public String getVersion();

  /**
   * Returns a description of the plugin from the version information.
   */
  public String getDescription();
}
