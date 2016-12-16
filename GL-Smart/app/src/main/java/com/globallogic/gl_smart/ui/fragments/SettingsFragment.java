package com.globallogic.gl_smart.ui.fragments;

import android.os.Bundle;
import android.support.v7.widget.Toolbar;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.MenuItem;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

import com.globallogic.gl_smart.R;
import com.globallogic.gl_smart.ui.base.BaseFragment;
import com.globallogic.gl_smart.utils.MqttManager;
import com.globallogic.gl_smart.utils.Settings;

import org.eclipse.paho.android.service.MqttAndroidClient;
import org.eclipse.paho.client.mqttv3.IMqttActionListener;
import org.eclipse.paho.client.mqttv3.IMqttToken;
import org.eclipse.paho.client.mqttv3.MqttException;

/**
 * @author eugenii.samarskyi.
 */
public class SettingsFragment extends BaseFragment implements Toolbar.OnMenuItemClickListener, View.OnClickListener {

	private static final String TAG = SettingsFragment.class.getSimpleName();

	private EditText mGatewayView;
	private TextView mStatusTextView;
	private Button mConnectBtn;

	public static SettingsFragment newInstance() {
		return new SettingsFragment();
	}

	private MqttAndroidClient mqttAndroidClient;

	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
		return inflater.inflate(R.layout.f_settings, container, false);
	}

	@Override
	public void onViewCreated(View view, Bundle savedInstanceState) {
		mqttAndroidClient = MqttManager.self().getMqtt();

		mGatewayView = (EditText) view.findViewById(R.id.gateway);
		mGatewayView.setText(getString(R.string.server_address));

		Toolbar toolbar = (Toolbar) view.findViewById(R.id.toolbar);
		toolbar.inflateMenu(R.menu.m_settings);
		toolbar.setOnMenuItemClickListener(this);

		toolbar.setNavigationIcon(R.drawable.ic_chevron_left);
		toolbar.setNavigationOnClickListener(this);

		mStatusTextView = (TextView) view.findViewById(R.id.status);
		mStatusTextView.setText(mqttAndroidClient.isConnected() ? getString(R.string.message_connected) : getString(R.string.message_disconnected));

		mConnectBtn = (Button) view.findViewById(R.id.connect);
		mConnectBtn.setText(mqttAndroidClient.isConnected() ? getString(R.string.message_disconnect) : getString(R.string.message_connect));
		mConnectBtn.setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View view) {

				if (!mqttAndroidClient.isConnected()) {
					connect();
				} else {
					disconnect();
				}
			}
		});
	}

	@Override
	public boolean onMenuItemClick(MenuItem item) {
		if (R.id.done == item.getItemId()) {
			Settings.self.setGateway(mGatewayView.getText().toString());

			MqttManager.self().reset();

			return true;
		}

		return false;
	}

	private void connect() {
		try {
			mqttAndroidClient.connect(null, new IMqttActionListener() {
				@Override
				public void onSuccess(IMqttToken asyncActionToken) {
					mStatusTextView.setText(R.string.message_connected);
					mConnectBtn.setText(R.string.message_disconnect);
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
			mqttAndroidClient.disconnect(null, new IMqttActionListener() {
				@Override
				public void onSuccess(IMqttToken asyncActionToken) {
					mStatusTextView.setText(R.string.message_disconnected);
					mConnectBtn.setText(R.string.message_connect);
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
	public void onClick(View view) {
		getFragmentManager().popBackStack();
	}
}
