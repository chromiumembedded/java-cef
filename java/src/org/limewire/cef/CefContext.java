// Copyright (c) 2010 Lime Wire, LLC. All rights reserved. Use of this source
// code is governed by a BSD-style license that can be found in the LICENSE file.

package org.limewire.cef;

/**
 * 
 * Class used to manage the CEF context.
 * @author Marshall
 *
 */
public class CefContext {
	/**
	 * Initialize the context.
	 * @return true on success
	 */
	public static final boolean initialize(String cachePath) {
		try {
			System.loadLibrary("icudt42");
			System.loadLibrary("libcef");
            System.loadLibrary("CefWrapper");
            return N_Initialize(System.getProperty("sun.boot.library.path"), cachePath);
        } catch (UnsatisfiedLinkError err) {
            err.printStackTrace();
        }
        return false;
	}
	
	/**
	 * Shut down the context.
	 */
	public static final void shutdown() {
		System.runFinalization();
		try {
			N_Shutdown();
        } catch (UnsatisfiedLinkError err) {
            err.printStackTrace();
        }
	}
	
	/**
	 * Create a new browser window.
	 * @param handler The object that will receive browser events.
	 * @param url The initial URL to load in the browser window.
	 * @return A new browser object.
	 */
	public static final CefBrowser createBrowser(CefHandler handler, String url) {
		return new CefBrowser_N(handler, url);
	}
	
	/**
	 * Create a new V8 undefined value.
	 * @return The new object if successful.
	 */
	public static final CefV8Value createV8Undefined() {
		try {
			return N_CreateV8Undefined();
        } catch (UnsatisfiedLinkError err) {
            err.printStackTrace();
        }
        return null;
	}
	
	/**
	 * Create a new V8 null value.
	 * @return The new object if successful.
	 */
	public static final CefV8Value createV8Null() {
		try {
			return N_CreateV8Null();
        } catch (UnsatisfiedLinkError err) {
            err.printStackTrace();
        }
        return null;
	}
	
	/**
	 * Create a new V8 boolean value.
	 * @param val The value.
	 * @return The new object if successful.
	 */
	public static final CefV8Value createV8Bool(boolean val) {
		try {
			return N_CreateV8Bool(val);
        } catch (UnsatisfiedLinkError err) {
            err.printStackTrace();
        }
        return null;
	}
	
	/**
	 * Create a new V8 int value.
	 * @param val The value.
	 * @return The new object if successful.
	 */
	public static final CefV8Value createV8Int(int val) {
		try {
			return N_CreateV8Int(val);
        } catch (UnsatisfiedLinkError err) {
            err.printStackTrace();
        }
        return null;
	}
	
	/**
	 * Create a new V8 double value.
	 * @param val The value.
	 * @return The new object if successful.
	 */
	public static final CefV8Value createV8Double(double val) {
		try {
			return N_CreateV8Double(val);
        } catch (UnsatisfiedLinkError err) {
            err.printStackTrace();
        }
        return null;
	}
	
	/**
	 * Create a new V8 string value.
	 * @param val The value.
	 * @return The new object if successful.
	 */
	public static final CefV8Value createV8String(String val) {
		try {
			return N_CreateV8String(val);
        } catch (UnsatisfiedLinkError err) {
            err.printStackTrace();
        }
        return null;
	}
	
	/**
	 * Create a new V8 object value.
	 * @param user_data The object representing user data.
	 * @return The new object if successful.
	 */
	public static final CefV8Value createV8Object(Object user_data) {
		try {
			return N_CreateV8Object(user_data);
        } catch (UnsatisfiedLinkError err) {
            err.printStackTrace();
        }
        return null;
	}
	
	/**
	 * Create a new V8 array value.
	 * @return The new object if successful.
	 */
	public static final CefV8Value createV8Array() {
		try {
			return N_CreateV8Array();
        } catch (UnsatisfiedLinkError err) {
            err.printStackTrace();
        }
        return null;
	}
	
	/**
	 * Create a new V8 function value.
	 * @param name The name of the function.
	 * @param handler The handler that will be called when the function is executed.
	 * @return The new object if successful.
	 */
	public static final CefV8Value createV8Function(String name, CefV8Handler handler) {
		try {
			return N_CreateV8Function(name, handler);
        } catch (UnsatisfiedLinkError err) {
            err.printStackTrace();
        }
        return null;
	}
	
	private static final native boolean N_Initialize(String pathToJavaDLL, String cachePath);
	private static final native void N_Shutdown();
	private static final native CefV8Value N_CreateV8Undefined();
	private static final native CefV8Value N_CreateV8Null();
	private static final native CefV8Value N_CreateV8Bool(boolean val);
	private static final native CefV8Value N_CreateV8Int(int val);
	private static final native CefV8Value N_CreateV8Double(double val);
	private static final native CefV8Value N_CreateV8String(String val);
	private static final native CefV8Value N_CreateV8Object(Object user_data);
	private static final native CefV8Value N_CreateV8Array();
	private static final native CefV8Value N_CreateV8Function(String name, CefV8Handler handler);
}
