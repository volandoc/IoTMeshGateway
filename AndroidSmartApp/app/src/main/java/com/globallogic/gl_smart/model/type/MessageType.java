package com.globallogic.gl_smart.model.type;

/**
 * @author eugenii.samarskyi.
 */
public enum MessageType {
	Command("command"), Event("event"), Status("status"), Property("property"), All("#");

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

		return Property;
	}
}
