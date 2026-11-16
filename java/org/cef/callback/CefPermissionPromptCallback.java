package org.cef.callback;

import org.cef.handler.CefPermissionRequestResult;

public interface CefPermissionPromptCallback {
    void Continue(CefPermissionRequestResult result);
}
