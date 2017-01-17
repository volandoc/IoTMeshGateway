package com.globallogic.gl_smart.ui.fragments;

import android.util.Log;

import com.globallogic.gl_smart.App;
import com.globallogic.gl_smart.model.mqtt.Property;
import com.globallogic.gl_smart.model.mqtt.PropertyPayload;
import com.globallogic.gl_smart.model.mqtt.StatusMessage;
import com.globallogic.gl_smart.model.mqtt.Topic;
import com.globallogic.gl_smart.ui.base.BaseFragment;
import com.globallogic.gl_smart.utils.MqttManager;
import com.globallogic.gl_smart.utils.Utils;

import org.eclipse.paho.client.mqttv3.IMqttDeliveryToken;
import org.eclipse.paho.client.mqttv3.MqttCallback;
import org.eclipse.paho.client.mqttv3.MqttMessage;

/**
 * @author eugenii.samarskyi.
 */
public abstract class MqttFragment extends BaseFragment implements MqttCallback {

	private static final String TAG = MqttFragment.class.getSimpleName();

	protected abstract String[] getTopics();

	protected  void onEvent(){};

	protected  void onCommand(){};

	protected  void onProperty(Property property){};

	protected  void onStatus(Topic topic, StatusMessage message){};

	@Override
	public void onResume() {
		super.onResume();
		Log.d(TAG, "onResume: ");

		MqttManager.self().setCallback(this);
		if (getTopics() == null) {
			return;
		}
		for (int i = 0; i < getTopics().length; i++) {
			Log.d(TAG, "onResume, subscribing to " + getTopics()[i]);
			MqttManager.self().subscribe(getTopics()[i]);
		}
	}

	@Override
	public void onDestroy() {
		Log.d(TAG, "onDestroy: ");
		if (getTopics() != null) {
			for (int i = 0; i < getTopics().length; i++) {
				Log.d(TAG, "onDestroy, unSubscribing from " + getTopics()[i]);
				MqttManager.self().unSubscribe(getTopics()[i]);
			}
		}

		super.onDestroy();
	}

	@Override
	public void messageArrived(String topic, MqttMessage message) throws Exception {
		String mess = new String(message.getPayload());
		Log.d(TAG, "Topic: " + topic + ", Message: " + mess);

		Topic t = new Topic(topic);

		switch (t.getMessageType()) {
			case Status:
				onStatus(t, App.getGson().fromJson(mess, StatusMessage.class));
				break;
			case Property:
				Property property = new Property();

				// get property name from topic
				String[] arr = topic.split(Utils.SEPARATOR);
				property.name = arr[arr.length - 1];

				// get property value from payload
				PropertyPayload payload = new PropertyPayload(mess);
				property.value = payload.getValue();

				onProperty(property);
				break;
			case Command:
				onCommand();
				break;
			case Event:
				onEvent();
				break;
		}
	}

	@Override
	public void connectionLost(Throwable cause) {
		Log.e(TAG, "connectionLost: " + cause.getLocalizedMessage());
	}

	@Override
	public void deliveryComplete(IMqttDeliveryToken token) {
		Log.i(TAG, "deliveryComplete");
	}
}
