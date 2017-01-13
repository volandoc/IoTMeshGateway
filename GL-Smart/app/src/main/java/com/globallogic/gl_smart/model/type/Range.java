package com.globallogic.gl_smart.model.type;

/**
 * Created by mariia.sorokina.
 */
public class Range {

    private Double min;
    private Double max;

    public Range(double min, double max) {
        this.min = min;
        this.max = max;
    }

    public double min() {
        return min;
    }

    public double max() {
        return max;
    }
}
