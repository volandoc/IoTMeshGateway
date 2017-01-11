package com.globallogic.gl_smart.model.mqtt;

import com.google.gson.JsonArray;
import com.google.gson.annotations.SerializedName;

/**
 * @author eugenii.samarskyi.
 */
public class Capability {
	String name;
	String type;
	String descr;
	String lim_type;
	JsonArray lim_json;

	@SerializedName("default")
	String def;
	String rw;
}
