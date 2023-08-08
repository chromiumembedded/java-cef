package org.cef.event;

public class CefKeyEvent {
    /* id constants */
    public static final int KEY_PRESS = 1;
    public static final int KEY_RELEASE = 0;
    public static final int KEY_TYPE = 2;

    // intentionally leaving these public for now
    // may remove the getters, or maybe add setters, or maybe move to private
    // not sure yet
    public int keyCode;
    public int id;
    public int modifiers;
    public char keyChar;
    public long scancode;

    public CefKeyEvent(int id, int keyCode, char keyChar, int modifiers) {
        this.id = id;
        this.keyCode = keyCode;
        this.keyChar = keyChar;
        this.modifiers = modifiers;
    }

    public int getID() {
        return id;
    }

    public int getModifiers() {
        return modifiers;
    }

    public char getKeyChar() {
        return keyChar;
    }

    public int getKeyCode() {
        return keyCode;
    }
}
