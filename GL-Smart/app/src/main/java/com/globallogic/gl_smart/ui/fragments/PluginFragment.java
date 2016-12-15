package com.globallogic.gl_smart.ui.fragments;

import android.os.Bundle;
import android.support.v7.widget.SwitchCompat;
import android.support.v7.widget.Toolbar;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.CompoundButton;
import android.widget.ImageView;

import com.globallogic.gl_smart.R;
import com.globallogic.gl_smart.model.Plugin;
import com.globallogic.gl_smart.ui.base.BaseFragment;
import com.globallogic.gl_smart.utils.MqttManager;
import com.squareup.picasso.Picasso;

import org.eclipse.paho.client.mqttv3.IMqttDeliveryToken;
import org.eclipse.paho.client.mqttv3.MqttCallback;
import org.eclipse.paho.client.mqttv3.MqttException;
import org.eclipse.paho.client.mqttv3.MqttMessage;

/**
 * @author eugenii.samarskyi.
 */
public class PluginFragment extends BaseFragment implements View.OnClickListener, CompoundButton.OnCheckedChangeListener, MqttCallback {

	private static final String TAG = PluginFragment.class.getSimpleName();

	private Toolbar mToolbar;
	private Plugin mPlugin;
	private SwitchCompat mSwitchView;
	private ImageView mImageView;

	private static final String NEST_TOPIC = "A000000000000777/NestDevicesPlugin/nK1FIdFVSFe_8wbS1Sz1Qpw07GxLCYe9MJ3dXTbkGKpEmPo6TnJr6Q/command";
	private static final String ESP_TOPIC = "A000000000000777/Esp8266Plugin/command";

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

		mToolbar = (Toolbar) view.findViewById(R.id.toolbar);
		mToolbar.setTitle(mPlugin.name);

		mToolbar.setNavigationIcon(R.drawable.ic_chevron_left);
		mToolbar.setNavigationOnClickListener(this);

		mSwitchView = (SwitchCompat) view.findViewById(R.id.switcher);
		mSwitchView.setOnCheckedChangeListener(this);

		mImageView = (ImageView) view.findViewById(R.id.image);

		MqttManager.self().subscribe(this);
	}

	@Override
	public void onDestroy() {
		MqttManager.self().unSubscribe();

		super.onDestroy();
	}

	@Override
	public void onClick(View view) {
		getFragmentManager().popBackStack();
	}

	@Override
	public void onCheckedChanged(CompoundButton compoundButton, boolean on) {
		if (on) {
			sendMessage(mPlugin.name.equals("Esp8266Plugin") ? PAYLOAD_ON : PAYLOAD_ON_NEST,
					mPlugin.name.equals("Esp8266Plugin") ? ESP_TOPIC : NEST_TOPIC);
		} else {
			sendMessage(mPlugin.name.equals("Esp8266Plugin") ? PAYLOAD_OFF : PAYLOAD_OFF_NEST,
					mPlugin.name.equals("Esp8266Plugin") ? ESP_TOPIC : NEST_TOPIC);
		}
	}

	@Override
	public void connectionLost(Throwable cause) {

	}

	@Override
	public void messageArrived(String topic, MqttMessage message) throws Exception {
		Log.d(TAG, "Message: " + topic + ": " + new String(message.getPayload()));

		String s = new String(message.getPayload());
		s = s.replace("\\", "");
		Log.d(TAG, s);

		if (s.contains("https://www.dropcam.com/api/wwn.get_snapshot")) {
			int start = s.indexOf("https://www.dropcam.com/api/wwn.get_snapshot");
			int end = s.indexOf('"', start);

			Log.d(TAG, s.substring(start, end));
			mImageView.setVisibility(View.VISIBLE);

			Picasso.with(getActivity()).load(s.substring(start, end)).into(mImageView);
		}
	}

	@Override
	public void deliveryComplete(IMqttDeliveryToken token) {

	}

	private void sendMessage(String mess, String topic) {
		if (!MqttManager.self().getMqtt().isConnected()) {
			return;
		}

		MqttMessage message = new MqttMessage();
		message.setPayload(mess.getBytes());

		try {
			MqttManager.self().getMqtt().publish(topic, message);
		} catch (MqttException e) {
			Log.e(TAG, "publish failure: ");
		}
	}

	private static final String PAYLOAD_ON = "{\n" +
			"\"id\": \"1\",\n" +
			"\"payload\": {\n" +
			"   \"type\": \"command\",\n" +
			"   \"value\": \"SET\",\n" +
			"   \"cvalue\": \"PROPERTIES\",\n" +
			"   \"content\": [{\n" +
			"   \"name\": \"power\",\n" +
			"   \"value\": \"on\"\n" +
			"}]\n" +
			"},\n" +
			"\"reference\": \"\",\n" +
			"\"timestamp\": 1234123412\n" +
			"}";

	private static final String PAYLOAD_OFF = "{\n" +
			"\"id\": \"1\",\n" +
			"\"payload\": {\n" +
			"   \"type\": \"command\",\n" +
			"   \"value\": \"SET\",\n" +
			"   \"cvalue\": \"PROPERTIES\",\n" +
			"   \"content\": [{\n" +
			"   \"name\": \"power\",\n" +
			"   \"value\": \"off\"\n" +
			"}]\n" +
			"},\n" +
			"\"reference\": \"\",\n" +
			"\"timestamp\": 1234123412\n" +
			"}";

	private static final String PAYLOAD_GET_PROPERTIES = "{\n" +
			"\"id\":\"1\",\n" +
			"\"payload\":\"\n" +
			"{\n" +
			"     \"cvalue\":\"PROPERTIES\",\n" +
			"     \"type\":\"command\",\n" +
			"     \"value\":\"GET\",\n" +
			"     \"content\":\"[]\"\n" +
			"}\",\n" +
			"\"reference\":\"\",\n" +
			"\"timestamp\":1479995948\n" +
			"}";

	private static final String PAYLOAD_ON_NEST = "{\n" +
			"    \"id\":\"1\",\n" +
			"    \"payload\":\"\n" +
			"    {\n" +
			"        \\\"cvalue\\\":\\\"PROPERTIES\\\",\n" +
			"        \\\"type\\\":\\\"command\\\",\n" +
			"        \\\"value\\\":\\\"SET\\\",\n" +
			"        \\\"content\\\":\\\"[{\n" +
			"            \\\\\\\"name\\\\\\\":\\\\\\\"is_streaming\\\\\\\",\n" +
			"            \\\\\\\"value\\\\\\\":\\\\\\\"true\\\\\\\"\n" +
			"        }]\\\"\n" +
			"    }\",\n" +
			"    \"reference\":\"\",\n" +
			"    \"timestamp\":1479995948\n" +
			"}";

	private static final String PAYLOAD_OFF_NEST = "{\n" +
			"    \"id\":\"1\",\n" +
			"    \"payload\":\"\n" +
			"    {\n" +
			"        \\\"cvalue\\\":\\\"PROPERTIES\\\",\n" +
			"        \\\"type\\\":\\\"command\\\",\n" +
			"        \\\"value\\\":\\\"SET\\\",\n" +
			"        \\\"content\\\":\\\"[{\n" +
			"            \\\\\\\"name\\\\\\\":\\\\\\\"is_streaming\\\\\\\",\n" +
			"            \\\\\\\"value\\\\\\\":\\\\\\\"false\\\\\\\"\n" +
			"        }]\\\"\n" +
			"    }\",\n" +
			"    \"reference\":\"\",\n" +
			"    \"timestamp\":1479995948\n" +
			"}";


	private static final String PAYLOAD_GET_PROPERTIES_NEST = "{\n" +
			"    \"id\":\"1\",\n" +
			"    \"payload\":\"\n" +
			"    {\n" +
			"        \\\"cvalue\\\":\\\"PROPERTIES\\\",\n" +
			"        \\\"type\\\":\\\"command\\\",\n" +
			"        \\\"value\\\":\\\"GET\\\",\n" +
			"        \\\"content\\\":\\\"[]\\\"\n" +
			"    }\",\n" +
			"    \"reference\":\"\",\n" +
			"    \"timestamp\":1479995948\n" +
			"}";
}
