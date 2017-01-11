package com.globallogic.gl_smart.utils;

import android.content.Context;
import android.content.res.Resources;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

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

	@SuppressWarnings("unchecked")
	public static <T extends View> T inflate(ViewGroup parent, int resourceId) {
		return (T) LayoutInflater.from(parent.getContext()).inflate(resourceId, parent, false);
	}

	@SuppressWarnings("unchecked")
	public static <T extends View> T inflate(ViewGroup parent, int resourceId, boolean attachToRoot) {
		return (T) LayoutInflater.from(parent.getContext()).inflate(resourceId, parent, attachToRoot);
	}

	@SuppressWarnings("unchecked")
	public static <T extends View> T inflate(Context context, int resourceId) {
		return (T) LayoutInflater.from(context).inflate(resourceId, null, false);
	}

	public static void setEnabledHierarchically(View view, boolean enabled) {
		view.setEnabled(enabled);

		if (view instanceof ViewGroup) {

			for (int i = 0; i < ((ViewGroup) view).getChildCount(); i++) {
				View child = ((ViewGroup) view).getChildAt(i);
				setEnabledHierarchically(child, enabled);
			}
		}
	}
}
