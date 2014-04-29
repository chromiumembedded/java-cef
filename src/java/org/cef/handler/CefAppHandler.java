// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.handler;

import org.cef.callback.CefCommandLine;

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

   // TODO(JCEF) Add the other callback methods for CefApp (see cef_app.h).
}
