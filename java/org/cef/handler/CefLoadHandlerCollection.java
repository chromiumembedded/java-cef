// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.handler;

import org.cef.browser.CefBrowser;
import org.cef.browser.CefFrame;
import org.cef.network.CefRequest;

import java.util.ArrayList;

/**
 * Convenience class to handle multiple load handlers. 
 */
public class CefLoadHandlerCollection extends ArrayList<CefLoadHandler> implements CefLoadHandler {
        
    @Override
    public void onLoadingStateChange(CefBrowser browser, boolean isLoading, boolean canGoBack, boolean canGoForward) {
        for(CefLoadHandler handler:this){
            handler.onLoadingStateChange(browser, isLoading, canGoBack, canGoForward);
        }
    }

    @Override
    public void onLoadStart(CefBrowser browser, CefFrame frame, CefRequest.TransitionType transitionType) {
        for(CefLoadHandler handler:this){
            handler.onLoadStart(browser, frame, transitionType);
        }
    }

    @Override
    public void onLoadEnd(CefBrowser browser, CefFrame frame, int httpStatusCode) {
        for(CefLoadHandler handler:this){
            handler.onLoadEnd(browser, frame, httpStatusCode);
        }
    }

    @Override
    public void onLoadError(CefBrowser browser, CefFrame frame, ErrorCode errorCode, String errorText, String failedUrl) {
        for(CefLoadHandler handler:this){
            handler.onLoadError(browser, frame, errorCode, errorText, failedUrl);
        }
    }
    
}
