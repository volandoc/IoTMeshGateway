package com.globallogic.gl_smart.model.mqtt;

import com.globallogic.gl_smart.App;
import com.google.gson.JsonObject;

import java.util.Calendar;

/**
 * @author eugenii.samarskyi.
 */
public class PropertyMessage {
	public long time;
	public String id;
	public String ref;
	public JsonObject data;

	public PropertyMessage() {

	}

	public PropertyMessage(Property... p) {
		time = Calendar.getInstance().getTimeInMillis();

		data = new JsonObject();
		for (Property property : p) {
			if (property.value instanceof String) {
				data.addProperty(property.name, String.valueOf(property.value));
			} else if (property.value instanceof Number) {
				data.addProperty(property.name, (Number) property.value);
			} else if (property.value instanceof Boolean) {
				data.addProperty(property.name, (Boolean) property.value);
			}
		}
	}

	public String getAsJson() {
		return App.getGson().toJson(this, PropertyMessage.class);
	}
}
