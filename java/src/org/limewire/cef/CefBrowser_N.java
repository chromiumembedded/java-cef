// Copyright (c) 2010 Lime Wire, LLC. All rights reserved. Use of this source
// code is governed by a BSD-style license that can be found in the LICENSE file.

package org.limewire.cef;

import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.Rectangle;

class CefBrowser_N extends CefBrowser {
	// The value of these members will be retrieved when the CEF browser
	// is actually created.
	private String load_url = null;
	private CefHandler handler = null;
	
	// Used internally to track if the browser has been created.
    private boolean N_CefCreated = false;
    
    // Used internally to store a pointer to the CEF object.
    private long N_CefHandle = 0;
	
    // Constructor used for already existing browser instances.
    CefBrowser_N() {
    	N_CefCreated = true;
    }
    
    // Constructor used for pending browser instances.
	CefBrowser_N(CefHandler handler, String url) {
    	this.load_url = url;
    	this.handler = handler;
    }
    
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
	public void goBack() {
        try {
        	N_GoBack();
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
	}

    @Override
	public void goForward() {
        try {
        	N_GoForward();
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
		
	}
    
    @Override
    public long getWindowHandle() {
    	try {
        	return N_GetWindowHandle();
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
        return 0;
    }
    
    @Override
    public boolean isPopup() {
    	try {
        	return N_IsPopup();
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
        return false;
    }

    @Override
	public CefFrame getMainFrame() {
        try {
        	return N_GetMainFrame();
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
		return null;
	}
    
    @Override
	public CefFrame getFocusedFrame() {
    	try {
        	return N_GetFocusedFrame();
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
        return null;
	}
	
    @Override
	public void paint(Graphics g) {
        try {
        	N_Paint(getWidth(), getHeight());
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
	}
	
    @Override
	public void setBounds(int x, int y, int width, int height) {
		try {
        	N_SetWindowSize(width, height);
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
		super.setBounds(x, y, width, height);
	}
	
    @Override
	public void setBounds(Rectangle r) {
		setBounds(r.x, r.y, r.width, r.height);
	}
	
    @Override
	public void setSize(int width, int height) {
		try {
        	N_SetWindowSize(width, height);
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
		super.setSize(width, height);
	}
	
    @Override
	public void setSize(Dimension d) {
		setSize(d.width, d.height);
	}
	
	private final native void N_Destroy();
    private final native void N_GoBack();
    private final native void N_GoForward();
    private final native long N_GetWindowHandle();
    private final native boolean N_IsPopup();
    private final native CefFrame N_GetMainFrame();
    private final native CefFrame N_GetFocusedFrame();
    private final native void N_Paint(int width, int height);
    private final native void N_SetWindowSize(int width, int height);
}
