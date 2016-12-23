package com.globallogic.gl_smart.model.mqtt;

import android.text.TextUtils;

import com.globallogic.gl_smart.model.mqtt.type.MessageType;
import com.globallogic.gl_smart.model.mqtt.type.SenderType;
import com.globallogic.gl_smart.utils.Utils;


/**
 * @author eugenii.samarskyi.
 */
public class Topic {
	public String topic;

	public Topic() {

	}

	public Topic(String t) {
		topic = t;
	}

	public String get(SenderType type) {
		switch (type) {
			case Gateway:
				return topic.split(Utils.SEPARATOR)[0];
			case Plugin:
				return topic.split(Utils.SEPARATOR)[1];
			case Sensor:
				return topic.split(Utils.SEPARATOR)[2];
			default:
				return topic.split(Utils.SEPARATOR)[0];
		}
	}

	public String gateway(){
		return topic.split(Utils.SEPARATOR)[0];
	}

	public String plugin(){
		return topic.split(Utils.SEPARATOR)[1];
	}

	public String sensor(){
		return topic.split(Utils.SEPARATOR)[2];
	}

	public MessageType getMessageType() {
		if (topic == null) {
			return null;
		}

		String[] arr = topic.split(Utils.SEPARATOR);
		if (arr.length > 0) {
			return MessageType.fromString(arr[arr.length - 1]);
		}

		return null;
	}

	public static class Builder {
		StringBuilder builder;
		String gatewayId;
		String pluginId;
		String deviceId;
		MessageType type;

		public Builder gatewayId(String id) {
			gatewayId = id;
			return this;
		}

		public Builder pluginId(String id) {
			pluginId = id;
			return this;
		}

		public Builder deviceId(String id) {
			deviceId = id;
			return this;
		}

		public Builder type(MessageType t) {
			type = t;
			return this;
		}

		public Topic build() {
			if (TextUtils.isEmpty(gatewayId)) {
				throw new IllegalArgumentException("gatewayId cannot be empty");
			}

			if (type == null) {
				throw new IllegalArgumentException("massage type cannot be NULL");
			}

			builder = new StringBuilder();

			Topic topic = new Topic();
			builder.append(gatewayId);

			if (pluginId != null) {
				builder.append(Utils.SEPARATOR).append(pluginId);
			}

			if (deviceId != null) {
				builder.append(Utils.SEPARATOR).append(deviceId);
			}

			builder.append(Utils.SEPARATOR).append(type.name);

			topic.topic = builder.toString();

			return topic;
		}
	}
}
