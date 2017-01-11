package com.globallogic.gl_smart.model.type;

/**
 * Created by Batman on 11.01.2017.
 */

public enum PropertyType {

	Bool("boolean"), Int("int"), Float("float"), Char("char"), Str("string"), Url("url");

	public final String name;

	PropertyType(String name) {
		this.name = name;
	}

	public static PropertyType fromString(String type) {
		if (type == null) {
			return null;
		}

		for (PropertyType messageType : PropertyType.values()) {
			if (messageType.name.equals(type)) {
				return messageType;
			}
		}

		return null;
	}
}
