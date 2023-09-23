// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.handler;

import org.cef.browser.CefBrowser;

/**
 * Implement this interface to handle events related to audio playing.
 * The methods of this class will be called on the UI thread.
 */
public interface CefAudioHandler {
	int getAudioParameters(CefBrowser browser, Object params);
	
	void onAudioStreamStarted(CefBrowser browser, Object params, int channels);
	
	void onAudioStreamPacket(CefBrowser browser, float[][] data, int frames, long pts);
	
	void onAudioStreamStopped(CefBrowser browser);
	
	void onAudioStreamError(CefBrowser browser, String text);
}
