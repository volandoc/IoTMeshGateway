package com.globallogic.gl_smart;

import android.support.test.runner.AndroidJUnit4;

import com.globallogic.gl_smart.model.mqtt.Topic;
import com.globallogic.gl_smart.model.type.MessageType;
import com.globallogic.gl_smart.model.type.TopicType;

import org.junit.Assert;
import org.junit.Before;
import org.junit.Test;
import org.junit.runner.RunWith;


@RunWith(AndroidJUnit4.class)
public class TopicTest {

	static final String TAG = TopicTest.class.getSimpleName();

	@Before
	public void init() {

	}

	@Test
	public void sensor() {
		Topic topic = new Topic.Builder()
				.gatewayId("777777777")
				.pluginId("nest")
				.sensorId("bulb001")
				.type(MessageType.Command)
				.build();

		Assert.assertEquals("777777777/nest/bulb001/command", topic.topic);
	}

	@Test
	public void plugin() {
		Topic topic = new Topic.Builder()
				.gatewayId("777777777")
				.pluginId("nest")
				.type(MessageType.Command)
				.build();

		Assert.assertEquals("777777777/nest/command", topic.topic);
	}

	@Test
	public void gateway() {
		Topic topic = new Topic.Builder()
				.gatewayId("777777777")
				.type(MessageType.Command)
				.build();

		Assert.assertEquals("777777777/command", topic.topic);
	}

	@Test
	public void senderGw() {
		Topic topic = new Topic.Builder()
				.gatewayId("777777777")
				.type(MessageType.Command)
				.build();

		Assert.assertEquals(TopicType.Gateway, TopicType.fromString(topic.topic));
	}

	@Test
	public void senderPlugin() {
		Topic topic = new Topic.Builder()
				.gatewayId("777777777")
				.pluginId("nest")
				.type(MessageType.Command)
				.build();

		Assert.assertEquals(TopicType.Plugin, TopicType.fromString(topic.topic));
	}

	@Test
	public void senderSensor() {
		Topic topic = new Topic.Builder()
				.gatewayId("777777777")
				.pluginId("nest")
				.sensorId("bulb001")
				.type(MessageType.Command)
				.build();

		Assert.assertEquals(TopicType.Sensor, TopicType.fromString(topic.topic));
	}

	@Test
	public void senderUnknown() {
		Assert.assertEquals(null, TopicType.fromString("777777777"));
	}

	@Test
	public void senderNull() {
		Assert.assertNull(TopicType.fromString(null));
	}
}