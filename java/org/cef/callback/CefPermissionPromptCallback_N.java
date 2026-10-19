package org.cef.callback;

import org.cef.handler.CefPermissionRequestResult;

class CefPermissionPromptCallback_N extends CefNativeAdapter implements CefPermissionPromptCallback {
    CefPermissionPromptCallback_N() {}

    @Override
    protected void finalize() throws Throwable {
        Continue(CefPermissionRequestResult.DISMISS);
        super.finalize();
    }

    @Override
    public void Continue(CefPermissionRequestResult result) {
        try {
            N_Continue(getNativeRef(null), result.getNativeValue());
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
    }

    private final native void N_Continue(long self, int result);
}
