package org.cef.callback;

public interface CefMediaAccessCallback {
    void Continue(int allowedPermissions);
    void Cancel();
}
