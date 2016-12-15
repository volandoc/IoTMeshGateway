package com.globallogic.gl_smart.model;

import java.io.Serializable;

/**
 * @author eugenii.samarskyi.
 */
public class Plugin implements Serializable{
	public String gateway;
	public String name;
	public String status;

	public Plugin(String gateway, String name, String status) {
		this.gateway = gateway;
		this.name = name;
		this.status = status;
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
