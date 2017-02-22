package com.globallogic.gl_smart.ui;

import android.content.Intent;
import android.os.Bundle;
import android.support.v7.widget.Toolbar;

import com.globallogic.gl_smart.App;
import com.globallogic.gl_smart.R;
import com.globallogic.gl_smart.model.Node;
import com.globallogic.gl_smart.ui.base.AppActivity;
import com.globallogic.gl_smart.ui.base.ToolbarFragment;
import com.globallogic.gl_smart.ui.fragments.DeviceFragment;


public class NodeActivity extends AppActivity implements ToolbarFragment.ToolbarContainer {

	private static final String NODE_KEY = "NODE_KEY";

	public static Intent newInstance(Node node) {
		return new Intent(App.self(), NodeActivity.class).putExtra(NODE_KEY, node);
	}

	private Toolbar mToolbar;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.a_node);
		mToolbar = (Toolbar) findViewById(R.id.toolbar);

		if (savedInstanceState == null) {
			getSupportFragmentManager().beginTransaction()
					.replace(R.id.content, DeviceFragment.newInstance((Node) getIntent().getSerializableExtra(NODE_KEY)))
					.commit();
		}
	}

	@Override
	public Toolbar getToolbar() {
		return mToolbar;
	}
}
