package com.globallogic.gl_smart.ui.fragments;

import android.content.Context;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.support.v7.widget.Toolbar;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import com.globallogic.gl_smart.App;
import com.globallogic.gl_smart.R;
import com.globallogic.gl_smart.model.mqtt.StatusMessage;
import com.globallogic.gl_smart.model.mqtt.Topic;
import com.globallogic.gl_smart.model.type.MessageType;
import com.globallogic.gl_smart.ui.GatewayCallback;
import com.globallogic.gl_smart.ui.MainActivity;
import com.globallogic.gl_smart.utils.MqttManager;

import org.eclipse.paho.client.mqttv3.IMqttDeliveryToken;
import org.eclipse.paho.client.mqttv3.MqttCallback;
import org.eclipse.paho.client.mqttv3.MqttMessage;

/**
 * @author eugenii.samarskyi.
 */
public class GatewayFragment extends DeviceFragment implements MqttCallback {

	private static final String TAG = GatewayFragment.class.getSimpleName();

	public static Fragment newInstance() {
		return new GatewayFragment();
	}

	private Toolbar mToolbar;

	private GatewayCallback mGatewayCallback;

	private final String topic = new Topic.Builder()
			.gatewayId("+")
			.type(MessageType.Status)
			.build()
			.topic;

	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
		return inflater.inflate(R.layout.f_gateway, container, false);
	}

	@Override
	public void onViewCreated(View view, Bundle savedInstanceState) {
		super.onViewCreated(view, savedInstanceState);

		App.getHandler().postDelayed(new Runnable() {
			@Override
			public void run() {
				try {
					messageArrived("A000000000000001/status", new MqttMessage(statusMessage.getBytes()));
				} catch (Exception e) {
					Log.e(TAG, "run: " + e.getLocalizedMessage());
				}
			}
		}, 2000);
	}

	@Override
	public void onAttach(Context context) {
		super.onAttach(context);

		mGatewayCallback = (GatewayCallback) context;

		if (context instanceof MainActivity) {
			mToolbar = ((MainActivity) context).getToolbar();
			mToolbar.setTitle(null);
			mToolbar.setSubtitle(null);
			mToolbar.getMenu().clear();
		}
	}

	@Override
	public void onResume() {
		super.onResume();

		Log.d(TAG, "onResume: ");
		MqttManager.self().subscribe(topic, this);
	}

	@Override
	public void onDestroy() {
		Log.d(TAG, "onDestroy: ");

		MqttManager.self().unSubscribe(topic);

		super.onDestroy();
	}

	@Override
	public void connectionLost(Throwable cause) {
//		Log.v(TAG, "Connection was lost");
	}

	@Override
	public void messageArrived(String topic_, MqttMessage message) throws Exception {
		String mess = new String(message.getPayload());
		Log.d(TAG, "Topic: " + topic_ + ", Message: " + mess);

		Topic topic = new Topic(topic_);
		mToolbar.setTitle(topic.gateway());
//		mToolbar.setSubtitle(mess);

		mGatewayCallback.onGateway(topic.gateway());

		StatusMessage statusMessage = App.getGson().fromJson(mess, StatusMessage.class);

		mCapabilities = statusMessage.data;
		mListView.setAdapter(new Adapter());
	}

	@Override
	public void deliveryComplete(IMqttDeliveryToken token) {
		Log.d(TAG, "Delivery Complete!");
	}

	private final String statusMessage = "{\n" +
			"\t\"status\": \"online\",\n" +
			"\t\"data\": [{\n" +
			"\t\t\"name\": \"power\",\n" +
			"\t\t\"type\": \"string\",\n" +
			"\t\t\"descr\": \"LED power\",\n" +
			"\t\t\"lim_type\": \"enum\",\n" +
			"\t\t\"lim_json\": [\n" +
			"\t\t\t\"on\",\n" +
			"\t\t\t\"off\"\n" +
			"\t\t],\n" +
			"\t\t\"default\": \"off\",\n" +
			"\t\t\"rw\": \"rw\"\n" +
			"\t}, {\n" +
			"\t\t\"name\": \"temperature\",\n" +
			"\t\t\"descr\": \"Temperature, C\",\n" +
			"\t\t\"type\": \"string\",\n" +
			"\t\t\"lim_type\": \"range\",\n" +
			"\t\t\"lim_json\": [\n" +
			"\t\t\t\"-20.12\",\n" +
			"\t\t\t\"99.8\"\n" +
			"\t\t],\n" +
			"\t\t\"default\": \"10.0\",\n" +
			"\t\t\"rw\": \"r\"\n" +
			"\t}, {\n" +
			"\t\t\"name\": \"ip\",\n" +
			"\t\t\"descr\": \"Ip Address\",\n" +
			"\t\t\"type\": \"string\",\n" +
			"\t\t\"lim_type\": \"regexp\",\n" +
			"\t\t\"lim_json\": [\n" +
			"\t\t\t\"[abc]\"\n" +
			"\t\t],\n" +
			"\t\t\"default\": \"10.35.0.2\",\n" +
			"\t\t\"rw\": \"rw\"\n" +
			"\t}],\n" +
			"\t\"time\": 1234123412,\n" +
			"\t\"err\": \"errMsg\"\n" +
			"}";
}
