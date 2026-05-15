package org.cef.handler;

import java.util.EnumSet;

public enum CefPermissionRequestType {
    NONE(0),
    AR_SESSION(1 << 0),
    CAMERA_PAN_TILT_ZOOM(1 << 1),
    CAMERA_STREAM(1 << 2),
    CAPTURED_SURFACE_CONTROL(1 << 3),
    CLIPBOARD(1 << 4),
    TOP_LEVEL_STORAGE_ACCESS(1 << 5),
    DISK_QUOTA(1 << 6),
    LOCAL_FONTS(1 << 7),
    GEOLOCATION(1 << 8),
    HAND_TRACKING(1 << 9),
    IDENTITY_PROVIDER(1 << 10),
    IDLE_DETECTION(1 << 11),
    MIC_STREAM(1 << 12),
    MIDI_SYSEX(1 << 13),
    MULTIPLE_DOWNLOADS(1 << 14),
    NOTIFICATIONS(1 << 15),
    KEYBOARD_LOCK(1 << 16),
    POINTER_LOCK(1 << 17),
    PROTECTED_MEDIA_IDENTIFIER(1 << 18),
    REGISTER_PROTOCOL_HANDLER(1 << 19),
    STORAGE_ACCESS(1 << 20),
    VR_SESSION(1 << 21),
    WEB_APP_INSTALLATION(1 << 22),
    WINDOW_MANAGEMENT(1 << 23),
    FILE_SYSTEM_ACCESS(1 << 24),
    LOCAL_NETWORK_ACCESS(1 << 25),
    LOCAL_NETWORK(1 << 26),
    LOOPBACK_NETWORK(1 << 27);

    private final int mask;

    CefPermissionRequestType(int mask) {
        this.mask = mask;
    }

    public int getMask() {
        return mask;
    }

    public static EnumSet<CefPermissionRequestType> fromMask(int mask) {
        EnumSet<CefPermissionRequestType> result = EnumSet.noneOf(CefPermissionRequestType.class);
        for (CefPermissionRequestType type : values()) {
            if (type != NONE && (mask & type.mask) != 0) {
                result.add(type);
            }
        }
        if (result.isEmpty()) {
            result.add(NONE);
        }
        return result;
    }

    public static int toMask(EnumSet<CefPermissionRequestType> types) {
        int result = 0;
        for (CefPermissionRequestType type : types) {
            result |= type.mask;
        }
        return result;
    }
}
