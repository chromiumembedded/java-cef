// Copyright (c) 2010 Lime Wire, LLC. All rights reserved. Use of this source
// code is governed by a BSD-style license that can be found in the LICENSE file.

package org.limewire.cef;

public interface CefV8Value {
	/**
	 * Check the value type.
	 */
	public boolean isUndefined();
	
	/**
	 * Check the value type.
	 */
	public boolean isNull();
	
	/**
	 * Check the value type.
	 */
	public boolean isBool();
	
	/**
	 * Check the value type.
	 */
	public boolean isInt();
	
	/**
	 * Check the value type.
	 */
	public boolean isDouble();
	
	/**
	 * Check the value type.
	 */
	public boolean isString();
	
	/**
	 * Check the value type.
	 */
	public boolean isObject();
	
	/**
	 * Check the value type.
	 */
	public boolean isArray();
	
	/**
	 * Check the value type.
	 */
	public boolean isFunction();
	
	/**
	 * Return a primitive value type.  The underlying data will be converted to
	 * the requested type if necessary.
	 */
	public boolean getBoolValue();
	
	/**
	 * Return a primitive value type.  The underlying data will be converted to
	 * the requested type if necessary.
	 */
	public int getIntValue();
	
	/**
	 * Return a primitive value type.  The underlying data will be converted to
	 * the requested type if necessary.
	 */
	public double getDoubleValue();
	
	/**
	 * Return a primitive value type.  The underlying data will be converted to
	 * the requested type if necessary.
	 */
	public String getStringValue();

	/**
	 * Returns true if the object has a value with the specified identifier.
	 * This method is only available on objects. Arrays and functions are also objects.
	 * String- and integer-based keys can be used interchangeably with the framework
	 * converting between them as necessary. Keys beginning with "Cef::" and "v8::"
	 * are reserved by the system.
	 */
	public boolean hasValue(String key);
	
	/**
	 * Returns true if the object has a value with the specified identifier.
	 * This method is only available on objects. Arrays and functions are also objects.
	 * String- and integer-based keys can be used interchangeably with the framework
	 * converting between them as necessary. Keys beginning with "Cef::" and "v8::"
	 * are reserved by the system.
	 */
	public boolean hasValue(int index);

	/**
	 * Delete the value with the specified identifier.
	 * This method is only available on objects. Arrays and functions are also objects.
	 * String- and integer-based keys can be used interchangeably with the framework
	 * converting between them as necessary. Keys beginning with "Cef::" and "v8::"
	 * are reserved by the system.
	 */
	public boolean deleteValue(String key);
	
	/**
	 * Delete the value with the specified identifier.
	 * This method is only available on objects. Arrays and functions are also objects.
	 * String- and integer-based keys can be used interchangeably with the framework
	 * converting between them as necessary. Keys beginning with "Cef::" and "v8::"
	 * are reserved by the system.
	 */
	public boolean deleteValue(int index);

	/**
	 * Returns the value with the specified identifier.
	 * This method is only available on objects. Arrays and functions are also objects.
	 * String- and integer-based keys can be used interchangeably with the framework
	 * converting between them as necessary. Keys beginning with "Cef::" and "v8::"
	 * are reserved by the system.
	 */
	public CefV8Value getValue(String key);
	
	/**
	 * Returns the value with the specified identifier.
	 * This method is only available on objects. Arrays and functions are also objects.
	 * String- and integer-based keys can be used interchangeably with the framework
	 * converting between them as necessary. Keys beginning with "Cef::" and "v8::"
	 * are reserved by the system.
	 */
	public CefV8Value getValue(int index);

	/**
	 * Associate the value with the specified identifier.
	 * This method is only available on objects. Arrays and functions are also objects.
	 * String- and integer-based keys can be used interchangeably with the framework
	 * converting between them as necessary. Keys beginning with "Cef::" and "v8::"
	 * are reserved by the system.
	 */
	public boolean setValue(String key, CefV8Value value);
	
	/**
	 * Associate the value with the specified identifier.
	 * This method is only available on objects. Arrays and functions are also objects.
	 * String- and integer-based keys can be used interchangeably with the framework
	 * converting between them as necessary. Keys beginning with "Cef::" and "v8::"
	 * are reserved by the system.
	 */
	public boolean setValue(int index, CefV8Value value);

	/**
	 * Read the keys for the object's values into the specified vector. Integer-
	 * based keys will also be returned as strings.
	 * This method is only available on objects. Arrays and functions are also objects.
	 * String- and integer-based keys can be used interchangeably with the framework
	 * converting between them as necessary. Keys beginning with "Cef::" and "v8::"
	 * are reserved by the system.
	*/
	public String[] getKeys();

	/**
	 * Returns the user data, if any, specified when the object was created.
	 * This method is only available on objects. Arrays and functions are also objects.
	 * String- and integer-based keys can be used interchangeably with the framework
	 * converting between them as necessary. Keys beginning with "Cef::" and "v8::"
	 * are reserved by the system.
	 */
	public Object getUserData();

	/**
	 * Returns the number of elements in the array.
	 * This method is only available on arrays.
	 */
	public int getArrayLength();

	/**
	 * Returns the function name.
	 * This method is only available on functions.
	 */
	public String getFunctionName();

	/**
	 * Returns the function handler or NULL if not a CEF-created function.
	 * This method is only available on functions.
	 */
	public CefV8Handler getFunctionHandler();
	  
	/**
	 * Execute the function.
	 * This method is only available on functions.
	 * @param params Input and output parameters.
	 * @return True if execution succeeded.
	*/
	public boolean executeFunction(CefV8FunctionParams params);
}
