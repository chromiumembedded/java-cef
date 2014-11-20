// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.handler;

import org.cef.CefApp.CefAppState;
import org.cef.callback.CefCommandLine;
import org.cef.callback.CefSchemeRegistrar;

/**
 * Implement this interface to provide handler implementations. Methods will be
 * called by the process and/or thread indicated.
 */
public interface CefAppHandler {
  /**
   * Provides an opportunity to view and/or modify command-line arguments before
   * processing by CEF and Chromium. The |process_type| value will be empty for
   * the browser process. Be cautious when using this method to modify
   * command-line arguments for non-browser processes as this may result in
   * undefined behavior including crashes.
   * @param process_type type of process (empty for browser process).
   * @param command_line values of the command line.
   */
   public void onBeforeCommandLineProcessing(String process_type, CefCommandLine command_line);

   /**
    * Provides an opportunity to hook into the native shutdown process. This
    * method is invoked if the user tries to terminate the app by sending the
    * corresponding key code (e.g. on Mac: CMD+Q) or something similar. If you
    * want to proceed with the default behavior of the native system, return
    * false. If you want to abort the terminate or if you want to implement your
    * own shutdown sequence return true and do the cleanup on your own.
    * @return false to proceed with the default behavior, true to abort terminate
    */
  public boolean onBeforeTerminate();

  /**
   * Implement this method to get state changes of the CefApp. 
   * See {@link CefAppState} for a complete list of possible states.
   *
   * For example, this method can be used e.g. to get informed if CefApp has
   * completed its initialization or its shutdown process.
   *
   * @param state The current state of CefApp.
   */
  public void stateHasChanged(CefAppState state);

  /**
    * Provides an opportunity to register custom schemes. Do not keep a reference
    * to the |registrar| object. This method is called on the main thread for
    * each process and the registered schemes should be the same across all
    * processes.
    */
   public void onRegisterCustomSchemes(CefSchemeRegistrar registrar);

   // Inherited of CefBrowserProcessHandler
   /**
    * Called on the browser process UI thread immediately after the CEF context
    * has been initialized.
    */
   public void onContextInitialized();

   /**
    * Return the handler for printing on Linux. If a print handler is not
    * provided then printing will not be supported on the Linux platform.
    *
    * @return a reference to a print handler implementation
    */
   public CefPrintHandler getPrintHandler();
}
