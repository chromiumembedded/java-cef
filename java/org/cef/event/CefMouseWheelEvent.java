package org.cef.event;

public class CefMouseWheelEvent {
    /* id constants */
    public static final int WHEEL_UNIT_SCROLL = 0;
    public static final int WHEEL_BLOCK_SCROLL = 1;

    // intentionally leaving these public for now
    // may remove the getters, or maybe add setters, or maybe move to private
    // not sure yet
    public int id;
    public double delta;
    public int x;
    public int y;
    public int modifiers;
    public int amount = 32; // TODO: might want to make this change dependent on system?

    public CefMouseWheelEvent(int id, int x, int y, double delta, int modifiers) {
        this.id = id;
        this.x = x;
        this.y = y;
        this.delta = delta;
        this.modifiers = modifiers;
    }

    public int getScrollType() {
        return id;
    }

    public double getWheelRotation() {
        return delta;
    }

    public int getX() {
        return x;
    }

    public int getY() {
        return y;
    }

    public int getModifiers() {
        return modifiers;
    }

    public double getUnitsToScroll() {
        return amount * delta;
    }
}
