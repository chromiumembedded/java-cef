package org.cef.handler;

import java.util.EnumMap;
import java.util.EnumSet;
import java.util.Map;
import org.cef.browser.CefBrowser;
import org.cef.browser.CefFrame;
import org.cef.callback.CefMediaAccessCallback;
import org.cef.callback.CefPermissionPromptCallback;

public abstract class CefPermissionHandlerAdapter implements CefPermissionHandler {
    private final Map<CefPermissionRequestType, Boolean> promptPolicy = new EnumMap<>(CefPermissionRequestType.class);

    protected CefPermissionHandlerAdapter() {
        for (CefPermissionRequestType type : CefPermissionRequestType.values()) {
            promptPolicy.put(type, Boolean.FALSE);
        }
        promptPolicy.put(CefPermissionRequestType.NONE, Boolean.FALSE);
    }

    public CefPermissionHandlerAdapter setPermission(CefPermissionRequestType type, boolean allow) {
        promptPolicy.put(type, allow);
        return this;
    }

    public boolean isAllowed(CefPermissionRequestType type) {
        return Boolean.TRUE.equals(promptPolicy.get(type));
    }

    protected CefPermissionRequestResult evaluatePrompt(
            CefBrowser browser,
            long promptId,
            String requestingOrigin,
            EnumSet<CefPermissionRequestType> requestedPermissions) {

        for (CefPermissionRequestType type : requestedPermissions) {
            switch (type) {
                case AR_SESSION:
                case CAMERA_PAN_TILT_ZOOM:
                case CAMERA_STREAM:
                case CAPTURED_SURFACE_CONTROL:
                case CLIPBOARD:
                case TOP_LEVEL_STORAGE_ACCESS:
                case DISK_QUOTA:
                case LOCAL_FONTS:
                case GEOLOCATION:
                case HAND_TRACKING:
                case IDENTITY_PROVIDER:
                case IDLE_DETECTION:
                case MIC_STREAM:
                case MIDI_SYSEX:
                case MULTIPLE_DOWNLOADS:
                case NOTIFICATIONS:
                case KEYBOARD_LOCK:
                case POINTER_LOCK:
                case PROTECTED_MEDIA_IDENTIFIER:
                case REGISTER_PROTOCOL_HANDLER:
                case STORAGE_ACCESS:
                case VR_SESSION:
                case WEB_APP_INSTALLATION:
                case WINDOW_MANAGEMENT:
                case FILE_SYSTEM_ACCESS:
                case LOCAL_NETWORK_ACCESS:
                case LOCAL_NETWORK:
                case LOOPBACK_NETWORK:
                    if (!isAllowed(type)) {
                        return CefPermissionRequestResult.DENY;
                    }
                    break;
                case NONE:
                default:
                    return CefPermissionRequestResult.IGNORE;
            }
        }

        return CefPermissionRequestResult.ACCEPT;
    }

    @Override
    public boolean onShowPermissionPrompt(
            CefBrowser browser,
            long promptId,
            String requestingOrigin,
            EnumSet<CefPermissionRequestType> requestedPermissions,
            CefPermissionPromptCallback callback) {
        callback.Continue(evaluatePrompt(browser, promptId, requestingOrigin, requestedPermissions));
        return true;
    }

    @Override
    public boolean onRequestMediaAccessPermission(
            CefBrowser browser,
            CefFrame frame,
            String requestingOrigin,
            int requestedPermissions,
            CefMediaAccessCallback callback) {
        callback.Continue(requestedPermissions);
        return true;
    }

    @Override
    public void onDismissPermissionPrompt(
            CefBrowser browser,
            long promptId,
            CefPermissionRequestResult result) {
    }
}
