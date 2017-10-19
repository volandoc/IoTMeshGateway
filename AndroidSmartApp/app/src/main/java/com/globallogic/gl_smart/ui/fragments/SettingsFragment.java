package com.globallogic.gl_smart.ui.fragments;

import android.os.Bundle;
import android.support.design.widget.Snackbar;
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
import com.globallogic.gl_smart.ui.MainActivity;
import com.globallogic.gl_smart.ui.base.ToolbarFragment;
import com.globallogic.gl_smart.utils.MqttManager;
import com.globallogic.gl_smart.utils.Settings;
import com.globallogic.gl_smart.utils.Utils;

import org.eclipse.paho.client.mqttv3.IMqttActionListener;
import org.eclipse.paho.client.mqttv3.IMqttToken;
import org.eclipse.paho.client.mqttv3.MqttException;

/**
 * @author eugenii.samarskyi.
 */
public class SettingsFragment extends ToolbarFragment implements Toolbar.OnMenuItemClickListener, View.OnClickListener {

	private static final String TAG = SettingsFragment.class.getSimpleName();

	private Button mConnectView;
	private TextView mStatusView;
	private EditText mGatewayView;

	public static SettingsFragment newInstance() {
		return new SettingsFragment();
	}

	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
		return inflater.inflate(R.layout.f_settings, container, false);
	}

	@Override
	public void onViewCreated(View view, Bundle savedInstanceState) {
		super.onViewCreated(view,savedInstanceState);

		mToolbar.setTitle(getString(R.string.settings_title));

		mGatewayView = (EditText) view.findViewById(R.id.gateway);
		mGatewayView.setText(getString(R.string.settings_serverAddress));

		mStatusView = (TextView) view.findViewById(R.id.status);
		mStatusView.setText(MqttManager.self().isConnected()
				? getString(R.string.message_connected)
				: getString(R.string.message_disconnected));

		mConnectView = (Button) view.findViewById(R.id.connect);
		mConnectView.setText(MqttManager.self().isConnected()
				? getString(R.string.message_disconnect)
				: getString(R.string.message_connect));

		mConnectView.setOnClickListener(this);
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
			MqttManager.self().connect(new IMqttActionListener() {
				@Override
				public void onSuccess(IMqttToken asyncActionToken) {
					mStatusView.setText(R.string.message_connected);
					mConnectView.setText(R.string.message_disconnect);

					((MainActivity) getActivity()).setStatus(true);
				}

				@Override
				public void onFailure(IMqttToken asyncActionToken, Throwable exception) {
					Log.e(TAG, exception.getLocalizedMessage());

					if (Utils.isAttached(SettingsFragment.this)) {
						Snackbar.make(mGatewayView, exception.getLocalizedMessage(), Snackbar.LENGTH_LONG).show();
					}
				}
			});
		} catch (MqttException ex) {
			Log.e(TAG, "Connect failure: " + ex.getLocalizedMessage());
		}
	}

	private void disconnect() {
		try {
			MqttManager.self().disconnect(new IMqttActionListener() {
				@Override
				public void onSuccess(IMqttToken asyncActionToken) {
					mStatusView.setText(R.string.message_disconnected);
					mConnectView.setText(R.string.message_connect);

					((MainActivity) getActivity()).setStatus(false);
				}

				@Override
				public void onFailure(IMqttToken asyncActionToken, Throwable exception) {
					Log.e(TAG, "Disconnection Failure");

					if (Utils.isAttached(SettingsFragment.this)) {
						Snackbar.make(mGatewayView, exception.getLocalizedMessage(), Snackbar.LENGTH_LONG).show();
					}
				}
			});
		} catch (MqttException ex) {
			Log.e(TAG, "Disconnect failure: " + ex.getLocalizedMessage());
		}
	}

	@Override
	public void onClick(View v) {
		if (!MqttManager.self().isConnected()) {
			connect();
		} else {
			disconnect();
		}
	}
}
