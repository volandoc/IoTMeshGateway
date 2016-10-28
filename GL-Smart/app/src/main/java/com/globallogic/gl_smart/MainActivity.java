package com.globallogic.gl_smart;

import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

import org.eclipse.paho.android.service.MqttAndroidClient;
import org.eclipse.paho.client.mqttv3.IMqttActionListener;
import org.eclipse.paho.client.mqttv3.IMqttDeliveryToken;
import org.eclipse.paho.client.mqttv3.IMqttToken;
import org.eclipse.paho.client.mqttv3.MqttAsyncClient;
import org.eclipse.paho.client.mqttv3.MqttCallback;
import org.eclipse.paho.client.mqttv3.MqttException;
import org.eclipse.paho.client.mqttv3.MqttMessage;

public class MainActivity extends AppCompatActivity implements MqttCallback {

    private static final String TAG = MainActivity.class.getSimpleName();

    private static final String TCP = "tcp://";
    private static final String PORT = ":1883";
    private static final String SUFFIX_SUB = "-sub";

    private static final String TOPIC = "#";
    private static final String NEW_LINE = "\n";

    private MqttAndroidClient mMqttAndroidClient;

    private TextView mStatusTextView;
    private Button mConnectBtn;
    private EditText mIpEditText;
    private TextView mLogTextView;
    private boolean mIsConnected = false;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        mStatusTextView = (TextView) findViewById(R.id.status_value);
        mConnectBtn = (Button) findViewById(R.id.connect);
        mLogTextView = (TextView) findViewById(R.id.log);
        mIpEditText = (EditText) findViewById(R.id.ip_address);

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
                        mMqttAndroidClient.subscribe(TOPIC, 0);
                        Log.d(TAG, "Subscribed to topic!");
                    } catch (MqttException e) {
                        Log.e(TAG, "Not subscribed: " + e.getLocalizedMessage());
                    }
                }

                @Override
                public void onFailure(IMqttToken asyncActionToken, Throwable exception) {
                    Log.d(TAG, "Connection Failure!");
                }
            });
        } catch (MqttException ex) {
            ex.printStackTrace();
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
                    Log.d(TAG, "Disconnection Failure!");
                }
            });

        } catch (MqttException ex) {
            ex.printStackTrace();
        }
    }

    @Override
    public void connectionLost(Throwable cause) {
        Log.d(TAG, "Connection was lost!");

    }

    @Override
    public void messageArrived(String topic, MqttMessage message) throws Exception {
        mLogTextView.append(new String(message.getPayload()) + NEW_LINE);
        Log.d(TAG, "Message: " + topic + ": " + new String(message.getPayload()));

    }

    @Override
    public void deliveryComplete(IMqttDeliveryToken token) {
        mMqttAndroidClient.acknowledgeMessage(String.valueOf(token.getMessageId()));
        Log.d(TAG, "Delivery Complete!");
    }
}