package com.globallogic.gl_smart.model;

import java.io.Serializable;

/**
 * @author eugenii.samarskyi.
 */
public class Sensor extends Node implements Serializable {
	public String plugin;

	public Sensor(String name, String status, String gateway, String plugin) {
		super(name, status, gateway);

		this.plugin = plugin;
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