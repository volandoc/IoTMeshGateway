package com.globallogic.gl_smart;

import android.support.test.runner.AndroidJUnit4;

import com.globallogic.gl_smart.model.mqtt.StatusMessage;

import org.json.JSONException;
import org.junit.Assert;
import org.junit.Test;
import org.junit.runner.RunWith;

/**
 * @author eugenii.samarskyi.
 */
@RunWith(AndroidJUnit4.class)
public class StatusMessageTest {

	@Test
	public void messageFromString() {
		StatusMessage message = App.getGson().fromJson(statusMessage, StatusMessage.class);

		Assert.assertNotNull(message);
	}

	@Test
	public void statusMessage() throws JSONException {
		StatusMessage message = App.getGson().fromJson(statusMessage, StatusMessage.class);

		Assert.assertEquals("online", message.status);

		Assert.assertEquals(3, message.data.size());
	}

	private final  String statusMessage = "{\n" +
			"\t\"status\": \"online\",\n" +
			"\t\"data\": [{\n" +
			"\t\t\"name\": \"power\",\n" +
			"\t\t\"type\": \"string\",\n" +
			"\t\t\"descr\": \"LED power\",\n" +
			"\t\t\"lim_type\": \"enum\",\n" +
			"\t\t\"lim_json\": [\n" +
			"\t\t\t\"on\",\n" +
			"\t\t\t\"off\"\n" +
			"\t\t],\n" +
			"\t\t\"default\": \"off\",\n" +
			"\t\t\"rw\": \"rw\"\n" +
			"\t}, {\n" +
			"\t\t\"name\": \"temperature\",\n" +
			"\t\t\"descr\": \"Temperature, C\",\n" +
			"\t\t\"type\": \"string\",\n" +
			"\t\t\"lim_type\": \"range\",\n" +
			"\t\t\"lim_json\": [\n" +
			"\t\t\t\"-20.12\",\n" +
			"\t\t\t\"99.8\"\n" +
			"\t\t],\n" +
			"\t\t\"default\": \"10.0\",\n" +
			"\t\t\"rw\": \"r\"\n" +
			"\t}, {\n" +
			"\t\t\"name\": \"ip\",\n" +
			"\t\t\"descr\": \"Ip Address\",\n" +
			"\t\t\"type\": \"string\",\n" +
			"\t\t\"lim_type\": \"regexp\",\n" +
			"\t\t\"lim_json\": [\n" +
			"\t\t\t\"[abc]\"\n" +
			"\t\t],\n" +
			"\t\t\"default\": \"10.35.0.2\",\n" +
			"\t\t\"rw\": \"rw\"\n" +
			"\t}],\n" +
			"\t\"time\": 1234123412,\n" +
			"\t\"err\": \"errMsg\"\n" +
			"}";
}
