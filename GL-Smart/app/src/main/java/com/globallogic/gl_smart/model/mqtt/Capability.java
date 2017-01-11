package com.globallogic.gl_smart.model.mqtt;

import com.google.gson.JsonArray;
import com.google.gson.annotations.SerializedName;

/**
 * @author eugenii.samarskyi.
 */
public class Capability {
	public String name;
	public String type;
	public String descr;
	public String lim_type;
	public JsonArray lim_json;

	@SerializedName("default")
	public String def;
	public String rw;
}
