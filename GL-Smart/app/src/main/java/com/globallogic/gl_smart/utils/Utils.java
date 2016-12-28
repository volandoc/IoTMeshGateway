package com.globallogic.gl_smart.utils;

import android.content.res.Resources;

/**
 * @author eugenii.samarskyi.
 */
public class Utils {
	public static final String SEPARATOR = "/";

	public static int getStatusBarHeight(Resources resources) {
		int result = 0;
		int resourceId = resources.getIdentifier("status_bar_height", "dimen", "android");
		if (resourceId > 0) {
			result = resources.getDimensionPixelSize(resourceId);
		}
		return result;
	}
}
