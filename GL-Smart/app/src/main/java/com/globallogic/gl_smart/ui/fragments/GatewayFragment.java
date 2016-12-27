package com.globallogic.gl_smart.ui.fragments;

import android.graphics.PorterDuff;
import android.graphics.drawable.Drawable;
import android.os.Bundle;
import android.support.design.widget.Snackbar;
import android.support.v4.app.Fragment;
import android.support.v4.content.ContextCompat;
import android.support.v7.widget.Toolbar;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.MenuItem;
import android.view.View;
import android.view.ViewGroup;

import com.globallogic.gl_smart.R;
import com.globallogic.gl_smart.model.mqtt.Topic;
import com.globallogic.gl_smart.model.mqtt.type.MessageType;
import com.globallogic.gl_smart.ui.base.BaseFragment;
import com.globallogic.gl_smart.utils.MqttManager;

import org.eclipse.paho.client.mqttv3.IMqttDeliveryToken;
import org.eclipse.paho.client.mqttv3.MqttCallback;
import org.eclipse.paho.client.mqttv3.MqttMessage;

import static com.globallogic.gl_smart.R.id.toolbar;

/**
 * @author eugenii.samarskyi.
 */
public class GatewayFragment extends BaseFragment implements MqttCallback, Toolbar.OnMenuItemClickListener, View.OnClickListener {

	private static final String TAG = GatewayFragment.class.getSimpleName();

	public static GatewayFragment newInstance() {
		return new GatewayFragment();
	}

	private String mGateway;
	private Toolbar mToolbar;

	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
		return inflater.inflate(R.layout.f_gateway, container, false);
	}

	@Override
	public void onViewCreated(View view, Bundle savedInstanceState) {
		mToolbar = (Toolbar) view.findViewById(toolbar);
		mToolbar.inflateMenu(R.menu.m_gateway);
		mToolbar.setOnMenuItemClickListener(this);

		Drawable drawable = ContextCompat.getDrawable(getActivity(), R.drawable.ic_circle);
		drawable.setColorFilter(MqttManager.self().isConnected()
						? getResources().getColor(android.R.color.holo_green_light)
						: getResources().getColor(android.R.color.holo_red_light),
				PorterDuff.Mode.SRC_IN);

		mToolbar.setNavigationIcon(drawable);

		view.findViewById(R.id.plugins).setOnClickListener(this);
		view.findViewById(R.id.sensors).setOnClickListener(this);

		Topic topic = new Topic.Builder()
				.gatewayId("+")
				.type(MessageType.Status)
				.build();

		MqttManager.self().subscribe(topic.topic, this);
	}

	@Override
	public void connectionLost(Throwable cause) {
//		Log.v(TAG, "Connection was lost");
	}

	@Override
	public void onDestroy() {
		MqttManager.self().unSubscribe();

		super.onDestroy();
	}

	@Override
	public void messageArrived(String topic_, MqttMessage message) throws Exception {
		String mess = new String(message.getPayload());
		Log.d(TAG, "Topic: " + topic_ + ", Message: " + mess);

		Topic topic = new Topic(topic_);
		mGateway = topic.gateway();
		mToolbar.setSubtitle(topic.gateway() + " " + mess);
	}

	@Override
	public void deliveryComplete(IMqttDeliveryToken token) {
		Log.d(TAG, "Delivery Complete!");
	}

	@Override
	public boolean onMenuItemClick(MenuItem item) {
		if (item.getItemId() == R.id.settings) {
			getActivity().getSupportFragmentManager()
					.beginTransaction()
					.setCustomAnimations(
							R.anim.enter_from_right, R.anim.exit_to_left,
							R.anim.enter_from_left, R.anim.exit_to_right
					)
					.replace(R.id.content, SettingsFragment.newInstance())
					.addToBackStack(null)
					.commit();
			return true;
		}

		return false;
	}

	@Override
	public void onClick(View v) {
		if (mGateway == null) {
			Snackbar.make(mToolbar, getString(R.string.message_noGateway), Snackbar.LENGTH_LONG).show();
			return;
		}

		Fragment fragment = null;

		if (R.id.sensors == v.getId()) {
			fragment = SensorListFragment.newInstance(mGateway);
		} else if (R.id.plugins == v.getId()) {
			fragment = PluginListFragment.newInstance(mGateway);
		}

		if (fragment == null) {
			return;
		}

		getActivity().getSupportFragmentManager()
				.beginTransaction()
				.setCustomAnimations(
						R.anim.enter_from_right, R.anim.exit_to_left,
						R.anim.enter_from_left, R.anim.exit_to_right
				)
				.replace(R.id.content, fragment)
				.addToBackStack(null)
				.commit();
	}
}
