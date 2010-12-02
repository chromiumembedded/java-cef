// Copyright (c) 2010 Lime Wire, LLC. All rights reserved. Use of this source
// code is governed by a BSD-style license that can be found in the LICENSE file.

package org.limewire.cef;

public class CefFrame_N implements CefFrame {
	 // Used internally to store a pointer to the CEF object.
    private long N_CefHandle = 0;
    
    CefFrame_N() {}
    
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
	public void loadURL(String url) {
        try {
        	N_LoadURL(url);
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
		
	}
    
    @Override
	public void executeJavaScript(String jsCode, String scriptUrl, int startLine) {
    	try {
        	N_ExecuteJavaScript(jsCode, scriptUrl, startLine);
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
	}
    
    @Override
	public boolean isMain() {
    	try {
        	return N_IsMain();
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
        return false;
	}

	@Override
	public boolean isFocused() {
		try {
        	return N_IsFocused();
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
        return false;
	}

	private final native void N_Destroy();
    private final native void N_LoadURL(String address);
    private final native void N_ExecuteJavaScript(String jsCode, String scriptUrl, int startLine);
    private final native boolean N_IsMain();
    private final native boolean N_IsFocused();
}
