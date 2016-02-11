package com.mapbox.mapboxsdk.maps;

public class ViewSettings {

    private boolean enabled;
    private int gravity;
    private int[]margins;

    public ViewSettings() {
        margins = new int[4];
    }

    public boolean isEnabled() {
        return enabled;
    }

    public void setEnabled(boolean enabled) {
        this.enabled = enabled;
    }

    public int getGravity() {
        return gravity;
    }

    public void setGravity(int gravity) {
        this.gravity = gravity;
    }

    public int[] getMargins() {
        return margins;
    }

    public void setMargins(int[] margins) {
        this.margins = margins;
    }
}
