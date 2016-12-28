package com.globallogic.gl_smart.ui.base;

import android.content.Context;
import android.support.v7.widget.Toolbar;

import com.globallogic.gl_smart.ui.MainActivity;

/**
 * @author eugenii.samarskyi.
 */
public class ToolbarFragment extends BaseFragment {

	private static final String TAG = ToolbarFragment.class.getSimpleName();

	protected Toolbar mToolbar;

	@Override
	public void onAttach(Context context) {
		super.onAttach(context);

		if (context instanceof MainActivity) {
			mToolbar = ((MainActivity) context).getToolbar();
			mToolbar.setTitle(null);
			mToolbar.setSubtitle(null);
			mToolbar.getMenu().clear();
		} else {
			throw new IllegalStateException("Wrong Activity, no toolbar");
		}
	}
}
