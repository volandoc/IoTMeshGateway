package com.globallogic.gl_smart.model;

import java.io.Serializable;

/**
 * @author eugenii.samarskyi.
 */
public class Node implements Serializable {
	public String name;
	public String status;
	public String gateway;

	public Node(String name, String status, String gateway) {
		this.name = name;
		this.status = status;
		this.gateway = gateway;
	}
}
