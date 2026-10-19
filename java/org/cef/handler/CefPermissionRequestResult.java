package org.cef.handler;

public enum CefPermissionRequestResult {
    ACCEPT(0),
    DENY(1),
    DISMISS(2),
    IGNORE(3);

    private final int nativeValue;

    CefPermissionRequestResult(int nativeValue) {
        this.nativeValue = nativeValue;
    }

    public int getNativeValue() {
        return nativeValue;
    }
}
