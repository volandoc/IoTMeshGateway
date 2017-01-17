package com.globallogic.gl_smart.model;

import java.io.Serializable;

/**
 * @author eugenii.samarskyi.
 */
public class Plugin extends Node implements Serializable {

	public Plugin(String name, String status, String gateway) {
		super(name, status, gateway);
	}

	@Override
	public String toString() {
		return "Plugin{" +
				"gateway='" + gateway + '\'' +
				", name='" + name + '\'' +
				", status='" + status + '\'' +
				'}';
	}
}
