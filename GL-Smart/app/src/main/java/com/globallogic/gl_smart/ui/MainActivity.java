package com.globallogic.gl_smart.ui;

import android.os.Bundle;

import com.globallogic.gl_smart.R;
import com.globallogic.gl_smart.ui.base.AppActivity;
import com.globallogic.gl_smart.ui.fragments.GatewayFragment;

public class MainActivity extends AppActivity {

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.a_main);

		if (savedInstanceState == null) {
			getSupportFragmentManager()
					.beginTransaction()
					.replace(R.id.content, GatewayFragment.newInstance())
					.commit();
		}
	}
}