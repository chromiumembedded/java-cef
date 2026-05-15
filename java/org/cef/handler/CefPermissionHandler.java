package org.cef.handler;

import java.util.EnumSet;
import org.cef.browser.CefBrowser;
import org.cef.browser.CefFrame;
import org.cef.callback.CefMediaAccessCallback;
import org.cef.callback.CefPermissionPromptCallback;

public interface CefPermissionHandler {
    boolean onRequestMediaAccessPermission(
            CefBrowser browser,
            CefFrame frame,
            String requestingOrigin,
            int requestedPermissions,
            CefMediaAccessCallback callback);

    boolean onShowPermissionPrompt(
            CefBrowser browser,
            long promptId,
            String requestingOrigin,
            EnumSet<CefPermissionRequestType> requestedPermissions,
            CefPermissionPromptCallback callback);

    void onDismissPermissionPrompt(
            CefBrowser browser,
            long promptId,
            CefPermissionRequestResult result);
}
