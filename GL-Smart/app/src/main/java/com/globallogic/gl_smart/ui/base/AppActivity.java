package com.globallogic.gl_smart.ui.base;

import android.support.v4.app.Fragment;
import android.support.v7.app.AppCompatActivity;

import java.util.List;

/**
 * @author eugenii.samarskyi.
 */
public class AppActivity extends AppCompatActivity {

	@Override
	public void onBackPressed() {
		if (!onBackPressedUser()) super.onBackPressed();
	}

	public boolean onBackPressedUser() {
		List<Fragment> fragments = getSupportFragmentManager().getFragments();
		if (fragments != null) {
			for (Fragment fragment : fragments) {
				if (fragment instanceof BaseFragment && ((BaseFragment) fragment).dispatchOnBackPressed()) {
					return true;
				}
			}
		}
		return false;
	}
}
