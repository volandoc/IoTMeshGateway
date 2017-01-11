package com.globallogic.gl_smart.model.type;

/**
 * Created by Batman on 11.01.2017.
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
