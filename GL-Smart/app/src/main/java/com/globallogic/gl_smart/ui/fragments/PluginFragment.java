package com.globallogic.gl_smart.ui.fragments;

import android.content.Intent;
import android.net.Uri;
import android.os.Bundle;
import android.support.design.widget.Snackbar;
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
import com.globallogic.gl_smart.model.mqtt.Topic;
import com.globallogic.gl_smart.model.type.MessageType;
import com.globallogic.gl_smart.model.type.SenderType;
import com.globallogic.gl_smart.ui.base.BaseFragment;
import com.globallogic.gl_smart.utils.MqttManager;
import com.squareup.picasso.Picasso;

import org.eclipse.paho.client.mqttv3.IMqttDeliveryToken;
import org.eclipse.paho.client.mqttv3.MqttCallback;
import org.eclipse.paho.client.mqttv3.MqttMessage;

/**
 * @author eugenii.samarskyi.
 */
public class PluginFragment extends BaseFragment implements View.OnClickListener, CompoundButton.OnCheckedChangeListener, MqttCallback {

	private static final String TAG = PluginFragment.class.getSimpleName();

	private Plugin mPlugin;
	private Toolbar mToolbar;
	private ImageView mImageView;
	private SwitchCompat mSwitchView;

	private static final String NEST_TOPIC = "A000000000000777/NestDevicesPlugin/nK1FIdFVSFe_8wbS1Sz1Qpw07GxLCYe9MJ3dXTbkGKpEmPo6TnJr6Q/command";
	private static final String ESP_TOPIC = "A000000000000777/Esp8266Plugin/LED/command";
	private static final String ESP_LED_TOPIC = "A000000000000777/Esp8266Plugin/LED/command";

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

		mToolbar = (Toolbar) view.findViewById(R.id.toolbar);
		mToolbar.setTitle(mPlugin.name);

		mToolbar.setNavigationIcon(R.drawable.ic_chevron_left);
		mToolbar.setNavigationOnClickListener(this);

		mSwitchView = (SwitchCompat) view.findViewById(R.id.switcher);
		mSwitchView.setOnCheckedChangeListener(this);

		mImageView = (ImageView) view.findViewById(R.id.image);

		Topic topic = new Topic.Builder()
				.gatewayId(mPlugin.gateway)
				.pluginId(mPlugin.name)
				.type(MessageType.All)
				.build();

		MqttManager.self().subscribe(topic.topic, this);
	}

	@Override
	public void onDestroy() {
		Log.d(TAG, "onDestroy: ");
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
			MqttManager.self().sendMessage(mPlugin.name.equals("Esp8266Plugin") ? PAYLOAD_ON : PAYLOAD_ON_NEST,
					mPlugin.name.equals("Esp8266Plugin") ? ESP_TOPIC : NEST_TOPIC);
		} else {
			MqttManager.self().sendMessage(mPlugin.name.equals("Esp8266Plugin") ? PAYLOAD_OFF : PAYLOAD_OFF_NEST,
					mPlugin.name.equals("Esp8266Plugin") ? ESP_TOPIC : NEST_TOPIC);
		}
	}

	@Override
	public void connectionLost(Throwable cause) {

	}

	@Override
	public void messageArrived(String topic, MqttMessage message) throws Exception {
		String mess = new String(message.getPayload());
		Log.d(TAG, "topic: " + topic + ", Mess: " + mess);

		String s = mess.replace("\\", "");

		if (s.contains("https://www.dropcam.com/api/wwn.get_snapshot")) {
			int start = s.indexOf("https://www.dropcam.com/api/wwn.get_snapshot");
			int end = s.indexOf('"', start);

			Log.d(TAG, s.substring(start, end));
			mImageView.setVisibility(View.VISIBLE);

			Picasso.with(getActivity()).load(s.substring(start, end)).into(mImageView);
		}

		Topic t = new Topic(topic);
		if (MessageType.Event == t.getMessageType() && SenderType.Sensor == SenderType.fromString(t.topic)) {
			Snackbar snackbar = Snackbar.make(mToolbar, mess, Snackbar.LENGTH_LONG)
					.setAction("Show camera", new View.OnClickListener() {
						@Override
						public void onClick(View v) {
							Intent intent = new Intent(Intent.ACTION_VIEW, Uri.parse(url));
							startActivity(intent);
						}
					});
			snackbar.show();
		}
	}

	@Override
	public void deliveryComplete(IMqttDeliveryToken token) {

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

	private static final String url = "https://home.nest.com/cameras/CjZDRDUwOU1oZ3U5Z3ZKdUNVNUFLbmJqb3ZDZXdMMzFZbWE5OVdBMklkR1VrUms1c2UxTzV6TWcSFkQ0aGZSOWlUanNMUnI5VDhWZEtkVncaNmVWYWFIalplV3JnN19TTVltX1RldDBDcWpjR0t3YkU5R0lGTzBEd01SbTBiOVdSdHZ3bXF3dw?auth=c.Bp7k52seS2eApXgSW2UPut9JySWNH059PoaV9ciQeMHfcT6p4BawHmF411knRYE4HmYQ4Rn3PpJcHRp9pe2Yv7bptMLegHFkT0LxOlu36uzlRotj24s14RRu9YU0H7fA72gFEQ43VUYJhkgt";
}
