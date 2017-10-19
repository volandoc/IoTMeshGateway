package com.globallogic.gl_smart.ui.base;

import android.support.v4.app.Fragment;

import java.util.List;

/**
 * @author eugenii.samarskyi.
 */
public class BaseFragment extends Fragment {

    private static final String TAG = BaseFragment.class.getSimpleName();

	public final boolean dispatchOnBackPressed() {
		List<Fragment> fragments = getChildFragmentManager().getFragments();
		if (fragments != null) {
			for (Fragment fragment : fragments) {
				if (fragment instanceof BaseFragment && ((BaseFragment) fragment).dispatchOnBackPressed()) {
					return true;
				}
			}
		}

		if (onBackPressed()) {
			return true;
		}

		if (getChildFragmentManager().getBackStackEntryCount() > 0) {
			getChildFragmentManager().popBackStackImmediate();
			return true;
		}

		return false;
	}

	public boolean onBackPressed() {
		return false;
	}

}
