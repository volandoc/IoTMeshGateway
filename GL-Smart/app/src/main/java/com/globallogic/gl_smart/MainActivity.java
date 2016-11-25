package com.globallogic.gl_smart;

import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.util.Log;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.TextView;

import com.squareup.picasso.Picasso;

import org.eclipse.paho.android.service.MqttAndroidClient;
import org.eclipse.paho.client.mqttv3.IMqttActionListener;
import org.eclipse.paho.client.mqttv3.IMqttDeliveryToken;
import org.eclipse.paho.client.mqttv3.IMqttToken;
import org.eclipse.paho.client.mqttv3.MqttAsyncClient;
import org.eclipse.paho.client.mqttv3.MqttCallback;
import org.eclipse.paho.client.mqttv3.MqttException;
import org.eclipse.paho.client.mqttv3.MqttMessage;

public class MainActivity extends AppCompatActivity implements MqttCallback, Toolbar.OnMenuItemClickListener {

	private static final String TAG = MainActivity.class.getSimpleName();

	private static final String TCP = "tcp://";
	private static final String PORT = ":1883";
	private static final String SUFFIX_SUB = "-sub";

	//	jTbPb7TCKrbGgfWaVonOJ5tVDqmZPyl8Y7rfJJ8fOG86xKGFjazY3g emulator
//	nK1FIdFVSFe_8wbS1Sz1Qpw07GxLCYe9MJ3dXTbkGKpEmPo6TnJr6Q real
	private static final String CURRENT_TOPIC = "A000000000000777/NestDevicesPlugin/nK1FIdFVSFe_8wbS1Sz1Qpw07GxLCYe9MJ3dXTbkGKpEmPo6TnJr6Q/command";
	private static final String PAYLOAD_ON = "{\n" +
			"    \"id\":\"1\",\n" +
			"    \"payload\":\"\n" +
			"    {\n" +
			"        \\\"cvalue\\\":\\\"PROPERTIES\\\",\n" +
			"        \\\"type\\\":\\\"command\\\",\n" +
			"        \\\"value\\\":\\\"SET\\\",\n" +
			"        \\\"content\\\":\\\"[{\n" +
			"            \\\\\\\"name\\\\\\\":\\\\\\\"is_streaming\\\\\\\",\n" +
			"            \\\\\\\"value\\\\\\\":\\\\\\\"true\\\\\\\"\n" +
			"        }]\\\"\n" +
			"    }\",\n" +
			"    \"reference\":\"\",\n" +
			"    \"timestamp\":1479995948\n" +
			"}";

	private static final String PAYLOAD_OFF = "{\n" +
			"    \"id\":\"1\",\n" +
			"    \"payload\":\"\n" +
			"    {\n" +
			"        \\\"cvalue\\\":\\\"PROPERTIES\\\",\n" +
			"        \\\"type\\\":\\\"command\\\",\n" +
			"        \\\"value\\\":\\\"SET\\\",\n" +
			"        \\\"content\\\":\\\"[{\n" +
			"            \\\\\\\"name\\\\\\\":\\\\\\\"is_streaming\\\\\\\",\n" +
			"            \\\\\\\"value\\\\\\\":\\\\\\\"false\\\\\\\"\n" +
			"        }]\\\"\n" +
			"    }\",\n" +
			"    \"reference\":\"\",\n" +
			"    \"timestamp\":1479995948\n" +
			"}";


	private static final String PAYLOAD_GET_PROPERTIES = "{\n" +
			"    \"id\":\"1\",\n" +
			"    \"payload\":\"\n" +
			"    {\n" +
			"        \\\"cvalue\\\":\\\"PROPERTIES\\\",\n" +
			"        \\\"type\\\":\\\"command\\\",\n" +
			"        \\\"value\\\":\\\"GET\\\",\n" +
			"        \\\"content\\\":\\\"[]\\\"\n" +
			"    }\",\n" +
			"    \"reference\":\"\",\n" +
			"    \"timestamp\":1479995948\n" +
			"}";

	private static final String ALL = "#";
	private static final String GW_EVENTS = "A000000000000777/+/+/event";

	private MqttAndroidClient mMqttAndroidClient;

	private TextView mStatusTextView;
	private Button mConnectBtn;
	private EditText mIpEditText;
	private TextView mLogTextView;
	private boolean mIsConnected = false;

	private ImageView mImageView;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);

		Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
		toolbar.inflateMenu(R.menu.m_main);
		toolbar.setOnMenuItemClickListener(this);

		mStatusTextView = (TextView) findViewById(R.id.status_value);
		mConnectBtn = (Button) findViewById(R.id.connect);
		mLogTextView = (TextView) findViewById(R.id.log);
		mIpEditText = (EditText) findViewById(R.id.ip_address);
		mImageView = (ImageView) findViewById(R.id.image);

		mConnectBtn.setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View view) {
				mLogTextView.setText(null);

				if (!mIsConnected) {
					init();
					connect();
				} else {
					disconnect();
				}
			}
		});
	}

	private void init() {
		String address = mIpEditText.getText().toString();
		String url = TCP + address + PORT;

		mMqttAndroidClient = new MqttAndroidClient(getApplicationContext(),
				url, MqttAsyncClient.generateClientId() + SUFFIX_SUB);
		mMqttAndroidClient.setCallback(this);
	}

	private void connect() {
		try {
			mMqttAndroidClient.connect(null, new IMqttActionListener() {
				@Override
				public void onSuccess(IMqttToken asyncActionToken) {
					mIsConnected = true;
					mStatusTextView.setText(R.string.status_connected);
					mConnectBtn.setText(R.string.disconnect);

					Log.d(TAG, "Connection Success!");
					Log.d(TAG, "Subscribing to topic...");
					try {
						mMqttAndroidClient.subscribe(ALL, 0);
//						mMqttAndroidClient.subscribe(GW_EVENTS, 0);
						Log.d(TAG, "Subscribed to all topics!");
					} catch (MqttException e) {
						Log.e(TAG, "Not subscribed:");
					}
				}

				@Override
				public void onFailure(IMqttToken asyncActionToken, Throwable exception) {
					Log.d(TAG, "Connection Failure!");
				}
			});
		} catch (MqttException ex) {
			Log.e(TAG, "connect: ");
		}
	}

	private void disconnect() {
		try {
			mMqttAndroidClient.disconnect(null, new IMqttActionListener() {
				@Override
				public void onSuccess(IMqttToken asyncActionToken) {
					Log.d(TAG, "Disconnected!");
					mIsConnected = false;
					mStatusTextView.setText(R.string.status_disconnected);
					mConnectBtn.setText(R.string.connect);
				}

				@Override
				public void onFailure(IMqttToken asyncActionToken, Throwable exception) {
					Log.e(TAG, "Disconnection Failure");
				}
			});

		} catch (MqttException ex) {
			Log.e(TAG, "disconnect");
		}
	}

	@Override
	public void connectionLost(Throwable cause) {
		Log.e(TAG, "Connection was lost");
	}

	@Override
	public void messageArrived(String topic, MqttMessage message) throws Exception {
		mLogTextView.setText(topic.concat(new String(message.getPayload())));
		Log.d(TAG, "Message: " + topic + ": " + new String(message.getPayload()));

		String s = new String(message.getPayload());
		s = s.replace("\\", "");
		Log.d(TAG, s);

		if (s.contains("https://www.dropcam.com/api/wwn.get_snapshot")) {
			int start = s.indexOf("https://www.dropcam.com/api/wwn.get_snapshot");
			int end = s.indexOf('"', start);

			Log.d(TAG, s.substring(start, end));
			mImageView.setVisibility(View.VISIBLE);

			Picasso.with(this).load(s.substring(start, end)).into(mImageView);
		}
	}

	@Override
	public void deliveryComplete(IMqttDeliveryToken token) {
		mMqttAndroidClient.acknowledgeMessage(String.valueOf(token.getMessageId()));
		Log.d(TAG, "Delivery Complete!");
	}

	@Override
	public boolean onMenuItemClick(MenuItem item) {
		if (mMqttAndroidClient == null || !mMqttAndroidClient.isConnected()) {
			return true;
		}

		MqttMessage message = new MqttMessage();
		switch (item.getItemId()) {
			case R.id.on:
				message.setPayload(PAYLOAD_ON.getBytes());
				try {
					mMqttAndroidClient.publish(CURRENT_TOPIC, message);
				} catch (MqttException e) {
					Log.e(TAG, "publish failure");
				}
				return true;

			case R.id.off:
				message.setPayload(PAYLOAD_OFF.getBytes());
				try {
					mMqttAndroidClient.publish(CURRENT_TOPIC, message);
				} catch (MqttException e) {
					Log.e(TAG, "publish failure: ");
				}
				return true;

			case R.id.get:
				message.setPayload(PAYLOAD_GET_PROPERTIES.getBytes());
				try {
					mMqttAndroidClient.publish(CURRENT_TOPIC, message);
				} catch (MqttException e) {
					Log.e(TAG, "publish failure: ");
				}
				return true;

			default:
				return false;
		}
	}
}