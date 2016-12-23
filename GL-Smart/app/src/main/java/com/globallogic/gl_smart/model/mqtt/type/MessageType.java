package com.globallogic.gl_smart.model.mqtt.type;

/**
 * @author eugenii.samarskyi.
 */
public enum MessageType {
	Command("command"), Event("event"), Status("status"), All("#");

	public final String name;

	MessageType(String n) {
		name = n;
	}

	public static MessageType fromString(String type) {
		if (type == null) {
			return null;
		}

		for (MessageType messageType : MessageType.values()) {
			if (messageType.name.equals(type)) {
				return messageType;
			}
		}

		return null;
	}
}
