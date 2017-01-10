package com.globallogic.gl_smart.utils;

import android.content.res.Resources;
import android.support.v4.app.Fragment;

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

	public static boolean isAttached(Fragment fragment) {
		return fragment != null && fragment.getActivity() != null && fragment.isAdded() && !fragment.isHidden();
	}
}
