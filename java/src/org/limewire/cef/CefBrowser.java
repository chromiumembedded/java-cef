// Copyright (c) 2010 Lime Wire, LLC. All rights reserved. Use of this source
// code is governed by a BSD-style license that can be found in the LICENSE file.

package org.limewire.cef;

import java.awt.Canvas;

public abstract class CefBrowser extends Canvas {
	/**
	 * Go back.
	 */
    public abstract void goBack();
    
    /**
     * Go forward.
     */
	public abstract void goForward();
	
	/**
	 * Returns the window handle for this browser.
	 */
	public abstract long getWindowHandle();
	
	/**
	 * Returns true if the window is a popup window.
	 */
	public abstract boolean isPopup();
	
	/**
	 * Returns the main (top-level) frame for the browser window.
	 */
	public abstract CefFrame getMainFrame();
	
	/**
	 * Returns the focused frame for the browser window.
	 */
	public abstract CefFrame getFocusedFrame();
}
