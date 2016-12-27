package com.globallogic.gl_smart.utils;

import android.util.Log;

import com.globallogic.gl_smart.App;

import org.eclipse.paho.android.service.MqttAndroidClient;
import org.eclipse.paho.client.mqttv3.IMqttActionListener;
import org.eclipse.paho.client.mqttv3.MqttAsyncClient;
import org.eclipse.paho.client.mqttv3.MqttCallback;
import org.eclipse.paho.client.mqttv3.MqttException;
import org.eclipse.paho.client.mqttv3.MqttMessage;

/**
 * @author eugenii.samarskyi.
 */
public class MqttManager {

	public static final String TAG = MqttManager.class.getSimpleName();

	private static final String TCP = "tcp://";
	private static final String PORT = ":1883";
	private static final String SUFFIX_SUB = "-sub";

	private static final String ALL = "#";

	private static MqttManager self;

	private MqttAndroidClient mqttAndroidClient;

	private MqttManager() {
		String url = TCP + Settings.self.getGateway() + PORT;
		mqttAndroidClient = new MqttAndroidClient(App.self(), url, MqttAsyncClient.generateClientId() + SUFFIX_SUB);

		Log.i(TAG, "Current url = " + url);
	}

	public MqttAndroidClient getMqtt() {
		return mqttAndroidClient;
	}

	public static MqttManager self() {
		if (self == null) {
			self = new MqttManager();
		}
		return self;
	}

	public void connect(IMqttActionListener l) throws MqttException {
		mqttAndroidClient.connect(null, l);
	}

	public void disconnect(IMqttActionListener l) throws MqttException {
		mqttAndroidClient.disconnect(null, l);
	}

	public void sendMessage(String mess, String topic) {
		if (!MqttManager.self().isConnected()) {
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

	public void reset() {
		if (mqttAndroidClient != null && mqttAndroidClient.isConnected()) {
			try {
				mqttAndroidClient.disconnect();
			} catch (MqttException e) {
				Log.e(TAG, "reset: ", e);
			}
		}
		mqttAndroidClient = null;
		self = null;
	}

	public boolean isConnected() {
		return mqttAndroidClient != null && mqttAndroidClient.isConnected();
	}

	public void unSubscribe() {
		if (mqttAndroidClient == null) {
			return;
		}

		if (mqttAndroidClient.isConnected()) {
			try {
				mqttAndroidClient.setCallback(null);
				mqttAndroidClient.unsubscribe(ALL);

				Log.i(TAG, "unSubscribed from " + ALL);
			} catch (MqttException e) {
				Log.e(TAG, "unSubscribe failed: ", e);
			}
		}
	}

	public void subscribe(MqttCallback callback) {
		subscribe(ALL, callback);
	}

	public void subscribe(String topic, MqttCallback callback) {
		if (mqttAndroidClient == null) {
			return;
		}
		mqttAndroidClient.setCallback(callback);

		if (mqttAndroidClient.isConnected()) {
			try {
				mqttAndroidClient.subscribe(topic, 0);
				Log.i(TAG, "subscribed on " + topic);
			} catch (MqttException e) {
				Log.e(TAG, "subscribe failed: ", e);
			}
		}
	}

	public void subscribe(String topic) {
		if (mqttAndroidClient == null) {
			return;
		}

		if (mqttAndroidClient.isConnected()) {
			try {
				mqttAndroidClient.subscribe(topic, 0);
				Log.i(TAG, "subscribed on " + topic);
			} catch (MqttException e) {
				Log.e(TAG, "subscribe failed: ", e);
			}
		}
	}

	public void setCallback(MqttCallback callback) {
		if (mqttAndroidClient == null) {
			return;
		}

		mqttAndroidClient.setCallback(callback);
	}
}
