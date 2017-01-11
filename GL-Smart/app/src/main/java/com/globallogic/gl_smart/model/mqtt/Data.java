package com.globallogic.gl_smart.model.mqtt;

import com.google.gson.JsonArray;
import com.google.gson.JsonElement;
import com.google.gson.JsonObject;

/**
 * @author eugenii.samarskyi.
 */
public class Data {
	public String type;
	public String value;
	public String cvalue;
	public JsonArray content;

	@Override
	public String toString() {
		return "Payload{" +
				"type='" + type + '\'' +
				", value='" + value + '\'' +
				", cvalue='" + cvalue + '\'' +
				", content=" + content +
				'}';
	}

	public void add(String key, String value) {
		if (content == null) {
			content = new JsonArray();
		}

		JsonObject o = new JsonObject();
		o.addProperty(key, value);
		content.add(o);
	}

	public void add(JsonElement element) {
		if (content == null) {
			content = new JsonArray();
		}

		content.add(element);
	}
}
