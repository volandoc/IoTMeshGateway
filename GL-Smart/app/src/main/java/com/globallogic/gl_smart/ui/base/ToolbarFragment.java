package com.globallogic.gl_smart.ui.base;

import android.os.Bundle;
import android.support.annotation.Nullable;
import android.support.v7.widget.Toolbar;
import android.view.View;

/**
 * @author eugenii.samarskyi.
 */
public abstract class ToolbarFragment extends BaseFragment {

	private static final String TAG = ToolbarFragment.class.getSimpleName();

	protected Toolbar mToolbar;

	public interface ToolbarContainer {
		Toolbar getToolbar();
	}

	@Override
	public void onViewCreated(View view, @Nullable Bundle savedInstanceState) {
		super.onViewCreated(view, savedInstanceState);
		mToolbar = ((ToolbarContainer) getActivity()).getToolbar();
		mToolbar.setTitle(null);
		mToolbar.setSubtitle(null);
		mToolbar.getMenu().clear();
	}
}
