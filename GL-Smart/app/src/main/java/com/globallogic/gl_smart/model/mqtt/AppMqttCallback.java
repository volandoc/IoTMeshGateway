package com.globallogic.gl_smart.model.mqtt;

import org.eclipse.paho.client.mqttv3.MqttCallback;
import org.eclipse.paho.client.mqttv3.MqttMessage;

/**
 * @author eugenii.samarskyi.
 */
public abstract class AppMqttCallback implements MqttCallback {

	public abstract void onMessageArrived(Topic t, String message);

	public abstract boolean isTopicSubscribed();

	@Override
	public void messageArrived(String topic, MqttMessage message) throws Exception {
		onMessageArrived(new Topic(topic), new String(message.getPayload()));
	}
}
