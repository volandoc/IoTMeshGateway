package com.globallogic.gl_smart.ui.fragments;

import android.os.Bundle;
import android.support.v7.widget.SwitchCompat;
import android.support.v7.widget.Toolbar;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;

import com.globallogic.gl_smart.R;
import com.globallogic.gl_smart.model.Plugin;
import com.globallogic.gl_smart.ui.base.BaseFragment;

/**
 * @author eugenii.samarskyi.
 */
public class PluginFragment extends BaseFragment {

	private static final String TAG = PluginFragment.class.getSimpleName();

	private Plugin mPlugin;
	private Toolbar mToolbar;
	private ImageView mImageView;
	private SwitchCompat mSwitchView;

	//	A000000000000001/ZWavePlugin_356238hhkj/#
	//	+/+/+/command
	public static PluginFragment newInstance(Plugin plugin) {
		Bundle args = new Bundle();
		args.putSerializable("plugin", plugin);

		PluginFragment fragment = new PluginFragment();
		fragment.setArguments(args);

		return fragment;
	}

	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
		return inflater.inflate(R.layout.f_plugin, container, false);
	}

	@Override
	public void onViewCreated(View view, Bundle savedInstanceState) {
		mPlugin = (Plugin) getArguments().getSerializable("plugin");
	}
}
