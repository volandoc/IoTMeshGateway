package com.globallogic.gl_smart.model.mqtt;

/**
 * @author eugenii.samarskyi.
 */
public class PropertyPayload {
	private String raw;
	private String[] data;

	public PropertyPayload(String string) {
		raw = string;
		data = raw.split(";");
	}

	public String getValue() {
		if (data.length == 4) {
			return data[0];
		} else {
			return raw;
		}
	}
}
