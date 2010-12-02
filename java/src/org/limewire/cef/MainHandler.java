// Copyright (c) 2010 Lime Wire, LLC. All rights reserved. Use of this source
// code is governed by a BSD-style license that can be found in the LICENSE file.

package org.limewire.cef;

public class MainHandler implements CefHandler {
	private MainFrame main_frame;
	private long main_handle = 0;

	public MainHandler(MainFrame main_frame) {
    	this.main_frame = main_frame;
    }
	
	@Override
	public void handleAfterCreated(CefBrowser browser) {
		if(!browser.isPopup())
			main_handle = browser.getWindowHandle();
	}
	
	@Override
	public void handleAddressChange(CefBrowser browser, CefFrame frame, String url) {
		if(browser.getWindowHandle() == main_handle && frame.isMain())
			main_frame.setAddress(url);
	}

	@Override
	public void handleTitleChange(CefBrowser browser, String title) {
		long handle = browser.getWindowHandle();
		if(handle == main_handle)
			main_frame.setTitle(title);
	}

	@Override
	public void handleJSBinding(CefBrowser browser, CefFrame frame, CefV8Value object) {
		// Add a "window.test" object.
		CefV8Value test_obj = CefContext.createV8Object(null);
		object.setValue("test", test_obj);
		
		// Add a "showMessage" function to the "window.test" object.
		test_obj.setValue("showMessage",
			CefContext.createV8Function("showMessage", new MainV8Handler(main_frame)));
	}
}
