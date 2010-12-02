// Copyright (c) 2010 Lime Wire, LLC. All rights reserved. Use of this source
// code is governed by a BSD-style license that can be found in the LICENSE file.

package org.limewire.cef;

public class CefV8FunctionParams {
	private CefV8Value object = null;
	private CefV8Value[] arguments = null;
	private CefV8Value retval = null;
	private String exception = null;
  
	CefV8FunctionParams() {
	}
	public CefV8FunctionParams(CefV8Value object, CefV8Value[] arguments) {
		this.object = object;
		this.arguments = arguments;
	}
  
	/**
	 * Return the object that this function is being executed on.
	 */
	public CefV8Value getObject() { return object; }
	
	/**
	 * Return the arguments passed to the function.
	 */
	public CefV8Value[] getArguments() { return arguments; }
	public boolean hasArguments() { return arguments.length > 0; }
  
	/**
	 * Set the return value.
	 */
	public void setRetVal(CefV8Value retval) { this.retval = retval; }
	
	/**
	 * Get the return value.
	 */
	public CefV8Value getRetVal() { return retval; }
  
	/**
	 * Set the exception, if any, that occurred during function execution.
	 */
	public void setException(String exception) { this.exception = exception; }
	
	/**
	 * Get the exception, if any, that occurred during function execution.
	 */
	public String getException() { return exception; }
}
