package com.globallogic.gl_smart.utils;

import android.app.Activity;
import android.content.Context;
import android.content.res.Resources;
import android.support.v4.app.Fragment;
import android.text.InputType;
import android.text.TextUtils;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.inputmethod.InputMethodManager;
import android.widget.TextView;

import com.globallogic.gl_smart.App;
import com.globallogic.gl_smart.model.mqtt.Capability;
import com.globallogic.gl_smart.model.type.PropertyType;
import com.globallogic.gl_smart.model.type.Range;
import com.globallogic.gl_smart.ui.view.AppSeekBar;

import static android.content.ContentValues.TAG;

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

	public static void showSoftKeyboard(View view) {
		view.requestFocus();
		InputMethodManager inputMethodManager = (InputMethodManager) view.getContext().getSystemService(Activity.INPUT_METHOD_SERVICE);
		inputMethodManager.showSoftInput(view, InputMethodManager.SHOW_IMPLICIT);
	}

	public static void hideSoftKeyboard(Activity activity) {
		InputMethodManager inputMethodManager = (InputMethodManager) activity.getSystemService(Activity.INPUT_METHOD_SERVICE);
		try {
			inputMethodManager.hideSoftInputFromWindow(activity.getCurrentFocus().getWindowToken(), 0);
		} catch (NullPointerException n) {
			Log.w(TAG, "hideSoftKeyboard: ", n);
		}
	}

	public static void hideSoftKeyboard(View view) {
		InputMethodManager inputMethodManager = (InputMethodManager) App.self().getSystemService(Activity.INPUT_METHOD_SERVICE);
		try {
			inputMethodManager.hideSoftInputFromWindow(view.getWindowToken(), 0);
		} catch (NullPointerException n) {
			Log.w(TAG, "hideSoftKeyboard: ", n);
		}
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

	public static void setWithProperty(AppSeekBar seekBar, TextView valueView, Capability capability, String value)
			throws NumberFormatException {

		PropertyType dataType = PropertyType.fromString(capability.type);
		switch (dataType) {
			case Int:
				seekBar.setDecimal(false);
				seekBar.setStep("1");
				break;
			case Float:
				seekBar.setDecimal(true);
				try {
					int dot = value.indexOf('.');
					if (value.contains("-")) {
						dot = dot - 1;
					}
					String step = "0.";
					for (int i = dot; i < value.length() - dot; i++) {
						step = step + "0";
					}
					step = step + "1";
					seekBar.setStep(step);
				} catch (Exception e) {
					seekBar.setStep("0.01");
				}
				break;
//			case INT_WITH_HALF:
//				seekBar.setDecimal(true);
//				seekBar.setStep("0.5");
//				break;
		}

		seekBar.setCurrentValueView(valueView);

		Range range = capability.getRange();
		seekBar.setShift(range.min());
		seekBar.setMax(range.max());
		seekBar.setMaxValue(range.max());
		seekBar.setMinValue(range.min());
//
		if (!TextUtils.isEmpty(value)) {
			Double progress = Double.valueOf(value);
			seekBar.setProgress(progress);
		} else {
			seekBar.setProgress(range.min());
		}
	}

	public static void setWithDataType(TextView valueView, PropertyType type) {
		if (type == null) {
			Log.e(TAG, "setWithDataType: type == null");
			return;
		}

		if (valueView == null) {
			Log.e(TAG, "setWithDataType: valueView == null");
			return;
		}

		switch (type) {
			case Int:
				valueView.setInputType(InputType.TYPE_CLASS_NUMBER);
				break;
			case Float:
				valueView.setInputType(InputType.TYPE_CLASS_NUMBER | InputType.TYPE_NUMBER_FLAG_DECIMAL);
				break;
			case Url:
				valueView.setInputType(InputType.TYPE_CLASS_TEXT | InputType.TYPE_TEXT_VARIATION_WEB_EMAIL_ADDRESS);
				break;
			default:
				valueView.setInputType(InputType.TYPE_CLASS_TEXT);
				break;

		}

//		valueView.setHint(Utils.getHintText(type));
	}
}
