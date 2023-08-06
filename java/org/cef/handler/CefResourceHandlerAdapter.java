// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.handler;

import org.cef.callback.CefCallback;
import org.cef.misc.BoolRef;
import org.cef.misc.IntRef;
import org.cef.misc.StringRef;
import org.cef.network.CefCookie;
import org.cef.network.CefRequest;
import org.cef.network.CefResponse;

/**
 * An abstract adapter class for receiving resource requests.
 * The methods in this class are empty.
 * This class exists as convenience for creating handler objects.
 */
public abstract class CefResourceHandlerAdapter implements CefResourceHandler {
    // forge causes some problems without this
    private static final ClassLoader clr;
    
    static {
        try {
            // reason: prevent class loading issues with forge
            Class<?>[] LOADER = new Class[] {
                    IntRef.class,
                    BoolRef.class,
                    CefRequest.class,
                    StringRef.class,
                    Class.forName("org.cef.callback.CefCallback_N"),
                    Class.forName("org.cef.network.CefResponse_N")
            };
        } catch (Throwable err) {
        }
        
        ClassLoader c = Thread.currentThread().getContextClassLoader();
        if (c == null) c = CefResourceHandlerAdapter.class.getClassLoader();
        clr = c;
    }
    
    /**
     * override {@link CefResourceHandlerAdapter#handleRequest(CefRequest, CefCallback)} instead
     */
    @Override
    public final boolean processRequest(CefRequest request, CefCallback callback) {
        Thread.currentThread().setContextClassLoader(clr); // reason: prevent class loading issues with forge
        return handleRequest(request, callback);
    }
    
    public boolean handleRequest(CefRequest request, CefCallback callback) {
        return false;
    }

    @Override
    public void getResponseHeaders(
            CefResponse response, IntRef responseLength, StringRef redirectUrl) {}

    @Override
    public boolean readResponse(
            byte[] dataOut, int bytesToRead, IntRef bytesRead, CefCallback callback) {
        return false;
    }

    @Override
    public void cancel() {}
}
