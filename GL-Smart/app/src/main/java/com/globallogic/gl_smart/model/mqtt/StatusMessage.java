package com.globallogic.gl_smart.model.mqtt;

import java.util.List;

/**
 * @author eugenii.samarskyi.
 */
public class StatusMessage extends Message {
	public String status;
	public List<Capability> data;
}
