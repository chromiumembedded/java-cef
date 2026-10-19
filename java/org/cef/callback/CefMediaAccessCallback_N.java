package org.cef.callback;

class CefMediaAccessCallback_N extends CefNativeAdapter implements CefMediaAccessCallback {
    CefMediaAccessCallback_N() {}

    @Override
    protected void finalize() throws Throwable {
        Cancel();
        super.finalize();
    }

    @Override
    public void Continue(int allowedPermissions) {
        try {
            N_Continue(getNativeRef(null), allowedPermissions);
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
    }

    @Override
    public void Cancel() {
        try {
            N_Cancel(getNativeRef(null));
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
    }

    private final native void N_Continue(long self, int allowedPermissions);
    private final native void N_Cancel(long self);
}
