// Copyright (c) 2010 Lime Wire, LLC. All rights reserved. Use of this source
// code is governed by a BSD-style license that can be found in the LICENSE file.

package org.limewire.cef;

public interface CefFrame {
	/**
	 * Load the specified url.
	 */
	public void loadURL(String url);
	
	/**
	 * Execute JavaScript.
	 * @param jsCode The JavaScript code to execute.
	 * @param scriptUrl The URL where the script in question can be found, if any.
	 * @param startLine The base line number to use for error reporting.
	 */
	public void executeJavaScript(String jsCode, String scriptUrl, int startLine);
	
	/**
	 * Returns true if this is the main frame.
	 */
	public boolean isMain();
	
	/**
	 * Returns true if this is the focused frame.
	 */
	public boolean isFocused();
}
