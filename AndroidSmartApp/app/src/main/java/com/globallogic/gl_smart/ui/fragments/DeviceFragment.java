package com.globallogic.gl_smart.ui.fragments;

import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import com.globallogic.gl_smart.App;
import com.globallogic.gl_smart.BuildConfig;
import com.globallogic.gl_smart.R;
import com.globallogic.gl_smart.model.Node;
import com.globallogic.gl_smart.model.Plugin;
import com.globallogic.gl_smart.model.Sensor;
import com.globallogic.gl_smart.model.mqtt.Topic;
import com.globallogic.gl_smart.model.type.MessageType;

import org.eclipse.paho.client.mqttv3.MqttMessage;

import static com.globallogic.gl_smart.ui.fragments.GatewayFragment.gwStatus;
import static com.globallogic.gl_smart.ui.fragments.GatewayFragment.status;

/**
 * @author eugenii.samarskyi.
 */
public class DeviceFragment extends NodeFragment implements View.OnClickListener {

	private static final String TAG = DeviceFragment.class.getSimpleName();
	private static final String NODE_KEY = "NODE_KEY";

	public static Fragment newInstance(Node node) {
		Bundle args = new Bundle();
		args.putSerializable(NODE_KEY, node);

		Fragment fragment = new DeviceFragment();
		fragment.setArguments(args);
		return fragment;
	}

	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

		mNode = (Node) getArguments().getSerializable(NODE_KEY);
	}

	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
		return inflater.inflate(R.layout.f_device, container, false);
	}

	@Override
	public void onViewCreated(View view, Bundle savedInstanceState) {
		super.onViewCreated(view, savedInstanceState);

		mToolbar.setNavigationIcon(R.drawable.ic_chevron_left);
		mToolbar.setNavigationOnClickListener(this);

		if (mNode != null) {
			mToolbar.setTitle(mNode.name);
			mToolbar.setSubtitle(mNode.status);
		}

		if (BuildConfig.BUILD_TYPE.equals("offlane")) {
			App.getHandler().postDelayed(new Runnable() {
				@Override
				public void run() {
					try {
						messageArrived(gwStatus, new MqttMessage(status.getBytes()));
					} catch (Exception e) {
						Log.e(TAG, "run: " + e.getLocalizedMessage());
					}
				}
			}, 2000);
		}
	}

	@Override
	protected String[] getTopics() {
		if (mTopic != null) {
			return new String[]{mTopic.topic};
		}

		Topic.Builder builder = new Topic.Builder();
		builder.type(MessageType.Status);
		if (mNode instanceof Plugin) {
			builder.pluginId(((Plugin) mNode).name);
			builder.gatewayId(((Plugin) mNode).gateway);
		} else if (mNode instanceof Sensor) {
			builder.sensorId(((Sensor) mNode).name);
			builder.pluginId(((Sensor) mNode).plugin);
			builder.gatewayId(((Sensor) mNode).gateway);
		}
		return new String[]{builder.build().topic};
	}

	@Override
	public void onClick(View v) {
		getActivity().finish();
	}
}
