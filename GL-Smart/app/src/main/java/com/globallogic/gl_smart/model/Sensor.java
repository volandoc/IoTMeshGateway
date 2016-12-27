package com.globallogic.gl_smart.model;

import java.io.Serializable;

/**
 * @author eugenii.samarskyi.
 */
public class Sensor implements Serializable {
	public String gateway;
	public String name;
	public String status;
	public String plugin;

	public Sensor(String gateway, String name, String plugin, String status) {
		this.name = name;
		this.plugin = plugin;
		this.status = status;
		this.gateway = gateway;
	}

	@Override
	public String toString() {
		return "Sensor{" +
				"gateway='" + gateway + '\'' +
				", name='" + name + '\'' +
				", status='" + status + '\'' +
				", plugin='" + plugin + '\'' +
				'}';
	}
}