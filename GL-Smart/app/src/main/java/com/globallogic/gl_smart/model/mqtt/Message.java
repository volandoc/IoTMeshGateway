package com.globallogic.gl_smart.model.mqtt;

import com.globallogic.gl_smart.App;

/**
 * @author eugenii.samarskyi.
 */
public class Message {
	public String id;
	public String reference;
	public long timestamp;
	public Payload payload;

	@Override
	public String toString() {
		return "Message{" +
				"id=" + id +
				", reference='" + reference + '\'' +
				", timestamp=" + timestamp +
				", payload=" + payload +
				'}';
	}

	public String toJson() {
		return App.getGson().toJson(this, Message.class);
	}
}
