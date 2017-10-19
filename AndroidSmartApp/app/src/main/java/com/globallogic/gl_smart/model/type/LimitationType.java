package com.globallogic.gl_smart.model.type;

/**
 * @author eugenii.samarskyi.
 */
public enum LimitationType {

	Range, Enum, List, Regexp, Null;

	public static LimitationType fromString(String type) {
		if (type == null) {
			return Null;
		}

		for (LimitationType messageType : LimitationType.values()) {
			if (messageType.name().toLowerCase().equals(type)) {
				return messageType;
			}
		}

		return Null;
	}
}
