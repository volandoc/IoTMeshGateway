package com.globallogic.gl_smart.ui;

import android.content.res.Configuration;
import android.graphics.PorterDuff;
import android.graphics.drawable.Drawable;
import android.os.Bundle;
import android.support.annotation.NonNull;
import android.support.design.widget.NavigationView;
import android.support.design.widget.Snackbar;
import android.support.v4.app.Fragment;
import android.support.v4.content.ContextCompat;
import android.support.v4.view.GravityCompat;
import android.support.v4.widget.DrawerLayout;
import android.support.v7.app.ActionBarDrawerToggle;
import android.support.v7.widget.Toolbar;
import android.util.Log;
import android.view.Gravity;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.ImageView;
import android.widget.TextView;

import com.globallogic.gl_smart.BuildConfig;
import com.globallogic.gl_smart.R;
import com.globallogic.gl_smart.ui.base.AppActivity;
import com.globallogic.gl_smart.ui.fragments.GatewayFragment;
import com.globallogic.gl_smart.ui.fragments.plugin.PluginListFragment;
import com.globallogic.gl_smart.ui.fragments.sensor.SensorListFragment;
import com.globallogic.gl_smart.ui.fragments.SettingsFragment;
import com.globallogic.gl_smart.utils.MqttManager;
import com.globallogic.gl_smart.utils.Utils;

import static com.globallogic.gl_smart.R.id.plugins;

public class MainActivity extends AppActivity implements NavigationView.OnNavigationItemSelectedListener, GatewayCallback {

	private static final String TAG = MainActivity.class.getSimpleName();

	private String mGateway;

	private Toolbar mToolbar;
	private DrawerLayout mDrawerLayout;
	private NavigationView mNavigationView;
	private ActionBarDrawerToggle mDrawerToggle;

	private ImageView mIndicatorView;
	private TextView mStatusView;

	private MenuItem mSensorsItem;
	private MenuItem mPluginsItem;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.a_main);

		mToolbar = (Toolbar) findViewById(R.id.toolbar);
		mDrawerLayout = (DrawerLayout) findViewById(R.id.drawer);
		mDrawerToggle = new ActionBarDrawerToggle(this, mDrawerLayout, mToolbar, R.string.open_drawable, R.string.close_drawable);

		mDrawerLayout.addDrawerListener(mDrawerToggle);

		mNavigationView = (NavigationView) findViewById(R.id.navigation);
		mNavigationView.setNavigationItemSelectedListener(this);
		mNavigationView.setCheckedItem(R.id.gateway);

		mSensorsItem = mNavigationView.getMenu().findItem(R.id.sensors);
		mSensorsItem.setEnabled(false);

		mPluginsItem = mNavigationView.getMenu().findItem(R.id.plugins);
		mPluginsItem.setEnabled(false);

		View view = mNavigationView.getHeaderView(0);
		view.setPadding(0, Utils.getStatusBarHeight(getResources()), 0, 0);

		mIndicatorView = (ImageView) view.findViewById(R.id.indicator);
		mStatusView = (TextView) view.findViewById(R.id.status);

		((TextView) view.findViewById(R.id.version)).setText(String.format(getString(R.string.version), BuildConfig.VERSION_NAME));

		setStatus(MqttManager.self().isConnected());

		onNavigationItemSelected(mNavigationView.getMenu().findItem(R.id.gateway));
	}

	@Override
	public void onBackPressed() {
		if (mDrawerLayout.isDrawerOpen(GravityCompat.START)) {
			mDrawerLayout.closeDrawer(GravityCompat.START);
		} else {
			super.onBackPressed();
		}
	}

	public Toolbar getToolbar() {
		return mToolbar;
	}

	@Override
	protected void onPostCreate(Bundle savedInstanceState) {
		super.onPostCreate(savedInstanceState);
		mDrawerToggle.syncState();
	}

	@Override
	public void onConfigurationChanged(Configuration newConfig) {
		super.onConfigurationChanged(newConfig);
		mDrawerToggle.onConfigurationChanged(newConfig);
	}

	@Override
	public boolean onPrepareOptionsMenu(Menu menu) {
		return super.onPrepareOptionsMenu(menu);
	}

	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		return mDrawerToggle.onOptionsItemSelected(item) || super.onOptionsItemSelected(item);
	}

	@Override
	public boolean onNavigationItemSelected(@NonNull MenuItem item) {
		if (mGateway == null && (item.getItemId() == R.id.plugins || item.getItemId() == R.id.sensors)) {
			Snackbar.make(mDrawerLayout, getString(R.string.message_noGateway), Snackbar.LENGTH_LONG).show();
			return true;
		}

		Fragment fragment = null;
		switch (item.getItemId()) {
			case R.id.gateway:
				fragment = GatewayFragment.newInstance();
				break;

			case plugins:
				fragment = PluginListFragment.newInstance(mGateway);
				break;

			case R.id.sensors:
				fragment = SensorListFragment.newInstance(mGateway);
				break;

			case R.id.settings:
				fragment = SettingsFragment.newInstance();
				break;

			default:
				fragment = GatewayFragment.newInstance();
				break;
		}

		mDrawerLayout.closeDrawer(Gravity.LEFT);
		getSupportFragmentManager()
				.beginTransaction()
				.replace(R.id.content, fragment)
				.commit();

		return true;
	}

	public void setStatus(boolean is) {
		Drawable drawable = ContextCompat.getDrawable(this, R.drawable.ic_circle);
		drawable.setColorFilter(is
						? getResources().getColor(android.R.color.holo_green_light)
						: getResources().getColor(android.R.color.holo_red_light),
				PorterDuff.Mode.SRC_IN);

		mIndicatorView.setImageDrawable(drawable);
		mStatusView.setText(is ? getString(R.string.message_connected) : getString(R.string.message_disconnected));
	}


	@Override
	public void onGateway(String gateway) {
		Log.d(TAG, "onGateway: " + gateway);
		mGateway = gateway;

		if (mGateway != null) {
			mSensorsItem.setEnabled(true);
			mPluginsItem.setEnabled(true);
		}
	}
}