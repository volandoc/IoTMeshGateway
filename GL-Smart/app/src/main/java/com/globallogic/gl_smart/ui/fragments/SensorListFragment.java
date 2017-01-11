package com.globallogic.gl_smart.ui.fragments;

import android.content.Context;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ProgressBar;
import android.widget.TextView;

import com.globallogic.gl_smart.R;
import com.globallogic.gl_smart.model.Sensor;
import com.globallogic.gl_smart.model.mqtt.Topic;
import com.globallogic.gl_smart.model.type.MessageType;
import com.globallogic.gl_smart.model.type.SenderType;
import com.globallogic.gl_smart.ui.GatewayCallback;
import com.globallogic.gl_smart.ui.base.ToolbarFragment;
import com.globallogic.gl_smart.utils.MqttManager;

import org.eclipse.paho.client.mqttv3.IMqttDeliveryToken;
import org.eclipse.paho.client.mqttv3.MqttCallback;
import org.eclipse.paho.client.mqttv3.MqttMessage;

import java.util.ArrayList;
import java.util.List;

/**
 * @author eugenii.samarskyi.
 */
public class SensorListFragment extends ToolbarFragment implements MqttCallback {

	private static final String TAG = SensorListFragment.class.getSimpleName();

	public static Fragment newInstance(String gateway) {
		Bundle args = new Bundle();
		args.putString("gateway", gateway);

		Fragment fragment = new SensorListFragment();
		fragment.setArguments(args);

		return fragment;
	}

	private String mGwTopic;
	private String mGateway;
	private String mSensorTopic;

	private List<Sensor> mSensorList;

	private RecyclerView mListView;
	private ProgressBar mProgressBar;
	private GatewayCallback mCallback;

	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
		return inflater.inflate(R.layout.f_sensor_list, container, false);
	}

	@Override
	public void onAttach(Context context) {
		super.onAttach(context);

		mCallback = (GatewayCallback) context;
	}

	@Override
	public void onViewCreated(View view, Bundle savedInstanceState) {
		mToolbar.setTitle(getString(R.string.sensors_title));

		mGateway = getArguments().getString("gateway");

		mSensorList = new ArrayList<>();

		mListView = (RecyclerView) view.findViewById(R.id.list);
		mListView.setLayoutManager(new LinearLayoutManager(getActivity()));
		mListView.setAdapter(new Adapter());

		mProgressBar = (ProgressBar) view.findViewById(R.id.progress);
		mProgressBar.setVisibility(MqttManager.self().isConnected() ? View.VISIBLE : View.INVISIBLE);

		mSensorTopic = new Topic.Builder()
				.gatewayId(mGateway)
				.pluginId("+")
				.deviceId("+")
				.type(MessageType.Status)
				.build().topic;

		MqttManager.self().subscribe(mSensorTopic);

		mGwTopic = new Topic.Builder()
				.gatewayId(mGateway)
				.type(MessageType.Status)
				.build().topic;

		MqttManager.self().subscribe(mGwTopic);
		MqttManager.self().setCallback(this);
	}

	@Override
	public void connectionLost(Throwable cause) {
//		Log.v(TAG, "Connection was lost");
	}

	@Override
	public void onDestroy() {
		MqttManager.self().unSubscribe(mGwTopic);
		MqttManager.self().unSubscribe(mSensorTopic);

		super.onDestroy();
	}

	@Override
	public void messageArrived(String topic_, MqttMessage message) throws Exception {
		String mess = new String(message.getPayload());
		Log.d(TAG, "Topic: " + topic_ + ", Message: " + mess);

		Topic topic = new Topic(topic_);

		SenderType senderType = SenderType.fromString(topic.topic);
		if (SenderType.Sensor == senderType) {
			Sensor sensor = findByName(topic.sensor());
			if (sensor == null) {
				mSensorList.add(new Sensor(topic.gateway(), topic.sensor(), topic.plugin(), mess));
				mListView.getAdapter().notifyItemInserted(mSensorList.size() - 1);
			} else {
				sensor.status = mess;
				mListView.getAdapter().notifyItemChanged(mSensorList.indexOf(sensor));
			}
		} else if (SenderType.Gateway == senderType) {
//			mToolbar.setSubtitle(topic.gateway() + " " + mess);

			mCallback.onGateway(topic.gateway());
		}

		if (mSensorList.size() > 0) {
			mProgressBar.setVisibility(View.GONE);
		}
	}

	private Sensor findByName(String name) {
		if (mSensorList == null) {
			return null;
		}

		for (Sensor plugin : mSensorList) {
			if (plugin.name.equals(name)) {
				return plugin;
			}
		}

		return null;
	}

	@Override
	public void deliveryComplete(IMqttDeliveryToken token) {
		Log.d(TAG, "Delivery Complete!");
	}

	class Adapter extends RecyclerView.Adapter<Holder> implements View.OnClickListener {

		@Override
		public Holder onCreateViewHolder(ViewGroup parent, int viewType) {
			return new Holder(LayoutInflater.from(getActivity()).inflate(R.layout.v_plugin_item, parent, false));
		}

		@Override
		public void onBindViewHolder(Holder holder, int position) {
			Sensor sensor = mSensorList.get(position);

			holder.name.setText(sensor.name);
			holder.status.setText(sensor.status);

			holder.itemView.setTag(sensor);
			holder.itemView.setOnClickListener(this);
		}

		@Override
		public int getItemCount() {
			return mSensorList.size();
		}

		@Override
		public void onClick(View view) {
//			Plugin plugin = (Plugin) view.getTag();

//			getActivity().getSupportFragmentManager()
//					.beginTransaction()
//					.setCustomAnimations(
//							R.anim.enter_from_right, R.anim.exit_to_left,
//							R.anim.enter_from_left, R.anim.exit_to_right
//					)
//					.replace(R.id.content, PluginFragment.newInstance(plugin))
//					.addToBackStack(null)
//					.commit();
		}
	}

	class Holder extends RecyclerView.ViewHolder {
		final TextView name;
		final TextView status;

		Holder(View itemView) {
			super(itemView);

			name = (TextView) itemView.findViewById(R.id.name);
			status = (TextView) itemView.findViewById(R.id.status);
		}
	}
}
