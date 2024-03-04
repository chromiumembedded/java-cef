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
     * @return false to proceed with the default behavior, true to abort
     * terminate.
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
     * Called from any thread when work has been scheduled for the browser process
     * main (UI) thread. This callback should schedule a
     * CefApp.DoMessageLoopWork() call to happen on the main (UI) thread.
     * |delay_ms| is the requested delay in milliseconds. If |delay_ms| is <= 0
     * then the call should happen reasonably soon. If |delay_ms| is > 0 then the
     * call should be scheduled to happen after the specified delay and any
     * currently pending scheduled call should be cancelled.
     */
    public void onScheduleMessagePumpWork(long delay_ms);

    /**
     * Implement this method to provide app-specific behavior when an already
     * running app is relaunched with the same CefSettings.root_cache_path value.
     * For example, activate an existing app window or create a new app window.
     * |command_line| will be read-only. Do not keep a reference to
     * |command_line| outside of this method. Return true if the relaunch is
     * handled or false for default relaunch behavior. Default behavior will
     * create a new default styled Chrome window.
     *
     * To avoid cache corruption only a single app instance is allowed to run for
     * a given CefSettings.root_cache_path value. On relaunch the app checks a
     * process singleton lock and then forwards the new launch arguments to the
     * already running app process before exiting early. Client apps should
     * therefore check the CefInitialize() return value for early exit before
     * proceeding.
     */
    public boolean onAlreadyRunningAppRelaunch(
            CefCommandLine command_line, String current_directory);
}
