package com.globallogic.gl_smart.utils;

import android.util.Log;

import com.globallogic.gl_smart.App;

import org.eclipse.paho.android.service.MqttAndroidClient;
import org.eclipse.paho.client.mqttv3.MqttAsyncClient;
import org.eclipse.paho.client.mqttv3.MqttCallback;
import org.eclipse.paho.client.mqttv3.MqttException;

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

	private  MqttAndroidClient mqttAndroidClient;

	private MqttManager() {
		String url = TCP + Settings.self.getGateway() + PORT;
		mqttAndroidClient = new MqttAndroidClient(App.self(), url, MqttAsyncClient.generateClientId() + SUFFIX_SUB);
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
			} catch (MqttException e) {
				Log.e(TAG, "unSubscribe failed: ", e);
			}
		}
	}

	public void subscribe(MqttCallback callback) {
		if (mqttAndroidClient == null) {
			return;
		}

		if (mqttAndroidClient.isConnected()) {
			try {
				mqttAndroidClient.setCallback(callback);
				mqttAndroidClient.subscribe(ALL, 0);
			} catch (MqttException e) {
				Log.e(TAG, "subscribe failed: ", e);
			}
		}
	}
}
