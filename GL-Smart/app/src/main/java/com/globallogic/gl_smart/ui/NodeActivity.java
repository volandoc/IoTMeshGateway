package com.globallogic.gl_smart.ui;

import android.content.Intent;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;

import com.globallogic.gl_smart.App;
import com.globallogic.gl_smart.R;
import com.globallogic.gl_smart.model.Node;
import com.globallogic.gl_smart.ui.fragments.NodeFragment;


public class NodeActivity extends AppCompatActivity {

	public static Intent newInstance(Node node) {
		return new Intent(App.self(), NodeActivity.class).putExtra("node", node);
	}

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.a_node);

		if (savedInstanceState == null) {
			getSupportFragmentManager().beginTransaction()
					.replace(R.id.content, NodeFragment.newInstance((Node) getIntent().getSerializableExtra("node")))
					.commit();
		}
	}
}
