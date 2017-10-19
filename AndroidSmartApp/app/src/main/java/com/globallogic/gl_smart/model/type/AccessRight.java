package com.globallogic.gl_smart.model.type;

/**
 * @author eugenii.samarskyi.
 */
public enum AccessRight {

	Writable("rw"), ReadOnly("r"), WriteOnly("w");

	public final String name;

	AccessRight(String name) {
		this.name = name;
	}

	public boolean isReadOnly() {
		return this == ReadOnly;
	}

	public static AccessRight fromString(String type) {
		if (type == null) {
			return ReadOnly;
		}

		for (AccessRight right : AccessRight.values()) {
			if (right.name.equals(type)) {
				return right;
			}
		}

		return ReadOnly;
	}
}
