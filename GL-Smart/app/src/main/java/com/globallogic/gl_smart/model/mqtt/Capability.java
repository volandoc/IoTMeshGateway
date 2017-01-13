package com.globallogic.gl_smart.model.mqtt;

import com.globallogic.gl_smart.model.type.Range;
import com.google.gson.JsonArray;
import com.google.gson.JsonElement;
import com.google.gson.annotations.SerializedName;

/**
 * @author eugenii.samarskyi.
 */
public class Capability {
	public String name;
	public String type;
	public String descr;
	public String lim_type;
	public JsonElement lim_json;

	@SerializedName("default")
	public String def;
	public String rw;

	public JsonArray getLimitation() {
		if (lim_json.isJsonNull()) {
			return null;
		}

		return lim_json.getAsJsonArray();
	}

	public Range getRange() {
		double min = lim_json.getAsJsonArray().get(0).getAsDouble();
		double max = lim_json.getAsJsonArray().get(1).getAsDouble();
		return new Range(min, max);
	}
}
