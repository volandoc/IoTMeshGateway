package com.globallogic.gl_smart.model.type;

import com.globallogic.gl_smart.utils.Utils;

/**
 * @author eugenii.samarskyi.
 */
public enum SenderType {
	Gateway, Plugin, Sensor;

	public static SenderType fromString(String topic) {
		if (topic == null) {
			return null;
		}

		String[] arr = topic.split(Utils.SEPARATOR);

		if (arr.length == 2) {
			return Gateway;
		} else if (arr.length == 3) {
			return Plugin;
		} else if (arr.length == 4) {
			return Sensor;
		} else {
			return null;
		}
	}
}
