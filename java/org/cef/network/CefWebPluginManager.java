// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.network;

import org.cef.callback.CefWebPluginInfoVisitor;
import org.cef.callback.CefWebPluginUnstableCallback;

/**
 * Class used to manage web plugins.
 */
public abstract class CefWebPluginManager {

  // This CTOR can't be called directly. Call method create() instead.
  CefWebPluginManager() {
  }

  /**
   * Returns the global plugin manager.
   */
  public static final CefWebPluginManager getGlobalManager() {
    return CefWebPluginManager_N.getInstance();
  }

  /**
   * Visit web plugin information. Can be called on any thread in the browser
   * process.
   * @param visitor Called with plugin information when available.
   */
  public abstract void visitPlugins(CefWebPluginInfoVisitor visitor);

  /**
   * Cause the plugin list to refresh the next time it is accessed regardless
   * of whether it has already been loaded. Can be called on any thread in the
   * browser process.
   */
  public abstract void refreshPlugins();

  /**
   * Add a plugin path (directory + file). This change may not take affect until
   * after refreshPlugins() is called. Can be called on any thread in the
   * browser process.
   */
  public abstract void addPlugin(String path);

  /**
   * Add a plugin directory. This change may not take affect until after
   * refreshPlugins() is called. Can be called on any thread in the browser
   * process.
   */
  public abstract void addPluginDirectory(String directory);

  /**
   * Remove a plugin path (directory + file). This change may not take affect
   * until after CefRefreshWebPlugins() is called. Can be called on any thread in
   * the browser process.
   *
   * @bug http://www.magpcss.org/ceforum/viewtopic.php?f=6&t=6149&p=21075&hilit=CefRemoveWebPluginPath#p21075
   */
  public abstract void removePlugin(String path);

  /**
   * Unregister an internal plugin. This may be undone the next time
   * CefRefreshWebPlugins() is called. Can be called on any thread in the browser
   * process.
   */
  public abstract void unregisterInternalPlugin(String path);

  /**
   * Force a plugin to shutdown. Can be called on any thread in the browser
   * process but will be executed on the IO thread.
   */
  public abstract void forcePluginShutdown(String path);

  /**
   * Register a plugin crash. Can be called on any thread in the browser process
   * but will be executed on the IO thread.
   */
  public abstract void registerPluginCrash(String path);

  /**
   * Query if a plugin is unstable. Can be called on any thread in the browser
   * process.
   */
  public abstract void isWebPluginUnstable(String path,
                                           CefWebPluginUnstableCallback callback);
}
