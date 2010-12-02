// Copyright (c) 2010 Lime Wire, LLC. All rights reserved. Use of this source
// code is governed by a BSD-style license that can be found in the LICENSE file.

package org.limewire.cef;

import javax.swing.JOptionPane;
import javax.swing.SwingUtilities;

public class MainV8Handler implements CefV8Handler {
	private MainFrame frame;
	
	MainV8Handler(MainFrame frame)
	{
		this.frame = frame;
	}
	
	// Class that will be used to show the message box on the event-dispatching thread.
	class ShowMessage implements Runnable
	{
		private String message;
		
		ShowMessage(String message) {
			this.message = message;
		}
		
		@Override
		public void run() {
			JOptionPane.showMessageDialog(frame, message);
		}
	}
	
	@Override
	public boolean executeFunction(String name, CefV8FunctionParams params) {
		if(name.equals("showMessage") && params.hasArguments())
		{
			// Retrieve the first argument as a string.
			String message = params.getArguments()[0].getStringValue();
			
			// Show the message box using the event dispatching thread.
			SwingUtilities.invokeLater(new ShowMessage(message));
			
			// Return a string value.
			params.setRetVal(CefContext.createV8String("Got the message."));
			
			return true;
		}
		return false;
	}
}
