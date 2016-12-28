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
import com.globallogic.gl_smart.ui.MainActivity;
import com.globallogic.gl_smart.ui.base.ToolbarFragment;
import com.globallogic.gl_smart.utils.MqttManager;
import com.globallogic.gl_smart.utils.Settings;

import org.eclipse.paho.client.mqttv3.IMqttActionListener;
import org.eclipse.paho.client.mqttv3.IMqttToken;
import org.eclipse.paho.client.mqttv3.MqttException;

/**
 * @author eugenii.samarskyi.
 */
public class SettingsFragment extends ToolbarFragment implements Toolbar.OnMenuItemClickListener, View.OnClickListener {

	private static final String TAG = SettingsFragment.class.getSimpleName();

	private EditText mGatewayView;
	private TextView mStatusTextView;
	private Button mConnectBtn;

	public static SettingsFragment newInstance() {
		return new SettingsFragment();
	}

	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
		return inflater.inflate(R.layout.f_settings, container, false);
	}

	@Override
	public void onViewCreated(View view, Bundle savedInstanceState) {
		mToolbar.setTitle(getString(R.string.settings_title));

		mGatewayView = (EditText) view.findViewById(R.id.gateway);
		mGatewayView.setText(getString(R.string.settings_serverAddress));

		mStatusTextView = (TextView) view.findViewById(R.id.status);
		mStatusTextView.setText(MqttManager.self().isConnected()
				? getString(R.string.message_connected)
				: getString(R.string.message_disconnected));

		mConnectBtn = (Button) view.findViewById(R.id.connect);
		mConnectBtn.setText(MqttManager.self().isConnected()
				? getString(R.string.message_disconnect)
				: getString(R.string.message_connect));

		mConnectBtn.setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View view) {
				if (!MqttManager.self().isConnected()) {
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
			MqttManager.self().connect(new IMqttActionListener() {
				@Override
				public void onSuccess(IMqttToken asyncActionToken) {
					mStatusTextView.setText(R.string.message_connected);
					mConnectBtn.setText(R.string.message_disconnect);

					((MainActivity)getActivity()).setStatus(true);
				}

				@Override
				public void onFailure(IMqttToken asyncActionToken, Throwable exception) {
					Log.e(TAG, "Connection Failure!");

//					Snackbar.make(mGatewayView, exception.getLocalizedMessage(), Snackbar.LENGTH_LONG).show();
				}
			});
		} catch (MqttException ex) {
			Log.e(TAG, "connect failure: " + ex.getLocalizedMessage());
		}
	}

	private void disconnect() {
		try {
			MqttManager.self().disconnect(new IMqttActionListener() {
				@Override
				public void onSuccess(IMqttToken asyncActionToken) {
					mStatusTextView.setText(R.string.message_disconnected);
					mConnectBtn.setText(R.string.message_connect);

					((MainActivity)getActivity()).setStatus(false);
				}

				@Override
				public void onFailure(IMqttToken asyncActionToken, Throwable exception) {
					Log.e(TAG, "Disconnection Failure");

//					Snackbar.make(mGatewayView, exception.getLocalizedMessage(), Snackbar.LENGTH_LONG).show();
				}
			});

		} catch (MqttException ex) {
			Log.e(TAG, "disconnect failure: " + ex.getLocalizedMessage());
		}
	}

	@Override
	public void onClick(View view) {
		getFragmentManager().popBackStack();
	}
}
