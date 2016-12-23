package com.globallogic.gl_smart;

import android.support.test.runner.AndroidJUnit4;

import com.globallogic.gl_smart.model.mqtt.Message;
import com.globallogic.gl_smart.model.mqtt.Payload;
import com.globallogic.gl_smart.model.mqtt.type.MessageType;
import com.google.gson.JsonElement;
import com.google.gson.JsonObject;

import org.json.JSONException;
import org.junit.Assert;
import org.junit.Test;
import org.junit.runner.RunWith;

/**
 * @author eugenii.samarskyi.
 */
@RunWith(AndroidJUnit4.class)
public class MessageTest {

	@Test
	public void messageFromString() {
		String m = "{\"id\":\"PIR01\",\"payload\": {\"type\":\"event\",\"value\":\"SUCCESS\",\"cvalue\":\"PROPERTIES\",\"сontent\":[{\"name\":\"PIR\",\"value\":\"0\"}]},\"reference\":\"1\",\"timestamp\" : 648116}";
		Message message = App.getGson().fromJson(m, Message.class);

		Assert.assertNotNull(message);
	}

	@Test
	public void testMessage() throws JSONException {
		Message message = new Message();
		message.id = "1";
		message.reference = null;
		message.timestamp = 1234123412;

		Payload payload = new Payload();
		payload.type = MessageType.Command.name;
		payload.value = "SET";
		payload.cvalue = "PROPERTIES";

		JsonObject object = new JsonObject();
		object.addProperty("name", "brightness");
		object.addProperty("value", "50");

		payload.add(object);

		object = new JsonObject();
		object.addProperty("name", "temperature");
		object.addProperty("value", "10");

		payload.add(object);
		message.payload = payload;

		JsonElement expected = App.getGson().fromJson(PAYLOAD_ON, JsonElement.class);
		JsonElement actual = App.getGson().fromJson(message.toJson(), JsonElement.class);

		Assert.assertEquals(expected, actual);
	}

	private static final String PAYLOAD_ON = "{\n" +
			"    \"id\":\"1\",\n" +
			"    \"payload\": {\n" +
			"        \"type\":\"command\",\n" +
			"        \"value\":\"SET\",\n" +
			"        \"cvalue\":\"PROPERTIES\",\n" +
			"        \"content\":[\n" +
			"           {\"name\":\"brightness\",\n" +
			"            \"value\":\"50\"},\n" +
			"           {\"name\":\"temperature\",\n" +
			"            \"value\":\"10\"}\n" +
			"        ]\n" +
			"    },\n" +
			"    \"reference\":null,\n" +
			"    \"timestamp\" : 1234123412\n" +
			"}";
}
