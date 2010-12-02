// Copyright (c) 2010 Lime Wire, LLC. All rights reserved. Use of this source
// code is governed by a BSD-style license that can be found in the LICENSE file.

package org.limewire.cef;

public class CefV8Value_N implements CefV8Value {
	// Used internally to store a pointer to the CEF object.
    private long N_CefHandle = 0;
	
    CefV8Value_N() {}
    
    @Override
	protected void finalize() throws Throwable
    {
    	try {
        	N_Destroy();
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
    	super.finalize();
    }
    
	@Override
	public boolean isUndefined() {
		try {
        	return N_IsUndefined();
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
		return false;
	}

	@Override
	public boolean isNull() {
		try {
        	return N_IsNull();
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
		return false;
	}

	@Override
	public boolean isBool() {
		try {
        	return N_IsBool();
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
		return false;
	}

	@Override
	public boolean isInt() {
		try {
        	return N_IsInt();
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
		return false;
	}

	@Override
	public boolean isDouble() {
		try {
        	return N_IsDouble();
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
		return false;
	}

	@Override
	public boolean isString() {
		try {
        	return N_IsString();
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
		return false;
	}

	@Override
	public boolean isObject() {
		try {
        	return N_IsObject();
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
		return false;
	}

	@Override
	public boolean isArray() {
		try {
        	return N_IsArray();
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
		return false;
	}

	@Override
	public boolean isFunction() {
		try {
        	return N_IsFunction();
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
		return false;
	}

	@Override
	public boolean getBoolValue() {
		try {
        	return N_GetBoolValue();
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
		return false;
	}

	@Override
	public int getIntValue() {
		try {
        	return N_GetIntValue();
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
		return 0;
	}

	@Override
	public double getDoubleValue() {
		try {
        	return N_GetDoubleValue();
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
		return 0;
	}

	@Override
	public String getStringValue() {
		try {
        	return N_GetStringValue();
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
		return null;
	}

	@Override
	public boolean hasValue(String key) {
		try {
        	return N_HasValue(key);
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
		return false;
	}

	@Override
	public boolean hasValue(int index) {
		try {
        	return N_HasValue(index);
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
		return false;
	}

	@Override
	public boolean deleteValue(String key) {
		try {
        	return N_DeleteValue(key);
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
		return false;
	}

	@Override
	public boolean deleteValue(int index) {
		try {
        	return N_DeleteValue(index);
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
		return false;
	}

	@Override
	public CefV8Value getValue(String key) {
		try {
        	return N_GetValue(key);
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
		return null;
	}

	@Override
	public CefV8Value getValue(int index) {
		try {
        	return N_GetValue(index);
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
		return null;
	}

	@Override
	public boolean setValue(String key, CefV8Value value) {
		try {
        	return N_SetValue(key, value);
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
		return false;
	}

	@Override
	public boolean setValue(int index, CefV8Value value) {
		try {
        	return N_SetValue(index, value);
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
		return false;
	}

	@Override
	public String[] getKeys() {
		try {
        	return N_GetKeys();
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
		return null;
	}

	@Override
	public Object getUserData() {
		try {
        	return N_GetUserData();
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
		return null;
	}

	@Override
	public int getArrayLength() {
		try {
        	return N_GetArrayLength();
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
		return 0;
	}

	@Override
	public String getFunctionName() {
		try {
        	return N_GetFunctionName();
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
		return null;
	}

	@Override
	public CefV8Handler getFunctionHandler() {
		try {
        	return N_GetFunctionHandler();
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
		return null;
	}

	@Override
	public boolean executeFunction(CefV8FunctionParams params) {
		try {
        	return N_ExecuteFunction(params);
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
		return false;
	}

	private final native void N_Destroy();
	private final native boolean N_IsUndefined();
	private final native boolean N_IsNull();
	private final native boolean N_IsBool();
	private final native boolean N_IsInt();
	private final native boolean N_IsDouble();
	private final native boolean N_IsString();
	private final native boolean N_IsObject();
	private final native boolean N_IsArray();
	private final native boolean N_IsFunction();
	private final native boolean N_GetBoolValue();
	private final native int N_GetIntValue();
	private final native double N_GetDoubleValue();
	private final native String N_GetStringValue();
	private final native boolean N_HasValue(String key);
	private final native boolean N_HasValue(int index);
	private final native boolean N_DeleteValue(String key);
	private final native boolean N_DeleteValue(int index);
	private final native CefV8Value N_GetValue(String key);
	private final native CefV8Value N_GetValue(int index);
	private final native boolean N_SetValue(String key, CefV8Value value);
	private final native boolean N_SetValue(int index, CefV8Value value);
	private final native String[] N_GetKeys();
	private final native Object N_GetUserData();
	private final native int N_GetArrayLength();
	private final native String N_GetFunctionName();
	private final native CefV8Handler N_GetFunctionHandler() ;
	private final native boolean N_ExecuteFunction(CefV8FunctionParams params);
}
