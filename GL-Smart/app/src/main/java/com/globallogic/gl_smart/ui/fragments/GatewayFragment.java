package com.globallogic.gl_smart.ui.fragments;

import android.content.Context;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import com.globallogic.gl_smart.R;
import com.globallogic.gl_smart.model.mqtt.Topic;
import com.globallogic.gl_smart.model.type.MessageType;
import com.globallogic.gl_smart.ui.GatewayCallback;
import com.globallogic.gl_smart.ui.base.ToolbarFragment;
import com.globallogic.gl_smart.utils.MqttManager;

import org.eclipse.paho.client.mqttv3.IMqttDeliveryToken;
import org.eclipse.paho.client.mqttv3.MqttCallback;
import org.eclipse.paho.client.mqttv3.MqttMessage;

/**
 * @author eugenii.samarskyi.
 */
public class GatewayFragment extends ToolbarFragment implements MqttCallback {

	private static final String TAG = GatewayFragment.class.getSimpleName();

	public static Fragment newInstance() {
		return new GatewayFragment();
	}

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
	public void onAttach(Context context) {
		super.onAttach(context);

		mGatewayCallback = (GatewayCallback) context;
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
		mToolbar.setSubtitle(mess);

		mGatewayCallback.onGateway(topic.gateway());
	}

	@Override
	public void deliveryComplete(IMqttDeliveryToken token) {
		Log.d(TAG, "Delivery Complete!");
	}
}
