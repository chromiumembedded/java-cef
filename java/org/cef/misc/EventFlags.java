package org.cef.misc;

/**
 * Supported event bit flags.
 */
public final class EventFlags {
    public final static int EVENTFLAG_NONE = 0;
    public final static int EVENTFLAG_CAPS_LOCK_ON = 1 << 0;
    public final static int EVENTFLAG_SHIFT_DOWN = 1 << 1;
    public final static int EVENTFLAG_CONTROL_DOWN = 1 << 2;
    public final static int EVENTFLAG_ALT_DOWN = 1 << 3;
    public final static int EVENTFLAG_LEFT_MOUSE_BUTTON = 1 << 4;
    public final static int EVENTFLAG_MIDDLE_MOUSE_BUTTON = 1 << 5;
    public final static int EVENTFLAG_RIGHT_MOUSE_BUTTON = 1 << 6;
    // Mac OS-X command key.
    public final static int EVENTFLAG_COMMAND_DOWN = 1 << 7;
    public final static int EVENTFLAG_NUM_LOCK_ON = 1 << 8;
    public final static int EVENTFLAG_IS_KEY_PAD = 1 << 9;
    public final static int EVENTFLAG_IS_LEFT = 1 << 10;
    public final static int EVENTFLAG_IS_RIGHT = 1 << 11;
}
