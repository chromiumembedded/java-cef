package org.cef.event;

public class CefMouseWheelEvent {
    /* id constants */
    public static final int WHEEL_UNIT_SCROLL = 0;
    public static final int WHEEL_BLOCK_SCROLL = 1;

    int id, delta, x, y, modifiers;
    int amount = 32; // TODO: might want to make this change dependent on system?

    public CefMouseWheelEvent(int id, int delta, int x, int y, int modifiers) {
        this.id = id;
        this.delta = delta;
        this.x = x;
        this.y = y;
        this.modifiers = modifiers;
    }

    public int getScrollType() {
        return id;
    }

    public int getWheelRotation() {
        return delta;
    }

    public int getX() {
        return x;
    }

    public int getY() {
        return y;
    }

    public int getModifiersEx() {
        return modifiers;
    }

    public int getUnitsToScroll() {
        return amount * delta;
    }
}
