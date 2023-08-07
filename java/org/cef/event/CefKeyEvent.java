package org.cef.event;

public class CefKeyEvent {
    public int keyCode;
    public int id;
    public int modifiersEx;
    public char keyChar;
    public long scancode;

    public CefKeyEvent(int keyCode, int id, int modifiersEx, char keyChar) {
        this.keyCode = keyCode;
        this.id = id;
        this.modifiersEx = modifiersEx;
        this.keyChar = keyChar;
    }

    public int getID() {
        return id;
    }

    public int getModifiersEx() {
        return modifiersEx;
    }

    public char getKeyChar() {
        return keyChar;
    }

    public int getKeyCode() {
        return keyCode;
    }
}
