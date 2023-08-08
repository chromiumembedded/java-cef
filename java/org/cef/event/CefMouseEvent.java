package org.cef.event;

public class CefMouseEvent {
    /* id constants */
    public static final int MOUSE_MOVED = 503;
    public static final int MOUSE_EXIT = 505;
    /* modifier constants */
    public static final int BUTTON1_MASK = 0x10;
    public static final int BUTTON2_MASK = 0x20;
    public static final int BUTTON3_MASK = 0x40;

    public int id;
    public int x;
    public int y;
    public int modifs;
    public int clickCount;
    public int button;

    public CefMouseEvent(int id, int x, int y, int modifs, int clickCount, int button) {
        this.id = id;
        this.x = x;
        this.y = y;
        this.modifs = modifs;
        this.clickCount = clickCount;
        this.button = button;
    }

    public int getID() {
        return id;
    }

    public int getX() {
        return x;
    }

    public int getY() {
        return y;
    }

    public int getModifiersEx() {
        return modifs;
    }

    public int getClickCount() {
        return clickCount;
    }

    public int getButton() {
        return button;
    }
}
