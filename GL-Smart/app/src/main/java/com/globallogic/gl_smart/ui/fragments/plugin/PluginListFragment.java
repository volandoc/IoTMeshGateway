package com.globallogic.gl_smart.ui.fragments.plugin;

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
import com.globallogic.gl_smart.model.Plugin;
import com.globallogic.gl_smart.model.mqtt.Topic;
import com.globallogic.gl_smart.model.type.MessageType;
import com.globallogic.gl_smart.model.type.TopicType;
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
public class PluginListFragment extends ToolbarFragment implements MqttCallback, View.OnClickListener {

	private static final String TAG = PluginListFragment.class.getSimpleName();

	public static Fragment newInstance(String gateway) {
		Bundle args = new Bundle();
		args.putString("gateway", gateway);

		Fragment fragment = new PluginListFragment();
		fragment.setArguments(args);

		return fragment;
	}

	private RecyclerView mListView;
	private ProgressBar mProgressBar;

	private String mGateway;
	private List<Plugin> mPluginList;

	private GatewayCallback mCallback;

	private String pluginsTopic;
	private String gwTopic;

	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
	}

	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
		return inflater.inflate(R.layout.f_plugin_list, container, false);
	}

	@Override
	public void onViewCreated(View view, Bundle savedInstanceState) {
		Log.d(TAG, "onViewCreated: SET TITLE");

		mToolbar.setTitle(getString(R.string.plugins_title));

		mGateway = getArguments().getString("gateway");

		pluginsTopic = new Topic.Builder()
				.gatewayId(mGateway)
				.pluginId("+")
				.type(MessageType.Status)
				.build().topic;

		gwTopic = new Topic.Builder()
				.gatewayId(mGateway)
				.type(MessageType.Status)
				.build().topic;

		mPluginList = new ArrayList<>();

		mListView = (RecyclerView) view.findViewById(R.id.list);
		mListView.setLayoutManager(new LinearLayoutManager(getActivity()));
		mListView.setAdapter(new Adapter());

		mProgressBar = (ProgressBar) view.findViewById(R.id.progress);
		mProgressBar.setVisibility(/*MqttManager.self().isConnected() ? View.VISIBLE : */View.INVISIBLE);
	}

	@Override
	public void connectionLost(Throwable cause) {
//		Log.v(TAG, "Connection was lost");
	}

	@Override
	public void onResume() {
		super.onResume();

		MqttManager.self().setCallback(this);
		MqttManager.self().subscribe(pluginsTopic);
		MqttManager.self().subscribe(gwTopic);
	}

	@Override
	public void onPause() {
		MqttManager.self().unSubscribe(pluginsTopic);
		MqttManager.self().unSubscribe(gwTopic);

		super.onPause();
	}

	@Override
	public void onAttach(Context context) {
		super.onAttach(context);

		mCallback = (GatewayCallback) context;
	}

	@Override
	public void messageArrived(String topic_, MqttMessage message) throws Exception {
		String mess = new String(message.getPayload());
		Log.d(TAG, "Topic: " + topic_ + ", Message: " + mess);

		Topic topic = new Topic(topic_);

		TopicType topicType = TopicType.fromString(topic.topic);
		if (TopicType.Plugin == topicType) {
			Plugin plugin = findByName(topic.plugin());
			if (plugin == null) {
				mPluginList.add(new Plugin(topic.gateway(), topic.plugin(), mess));
				mListView.getAdapter().notifyItemInserted(mPluginList.size() - 1);
			} else {
				plugin.status = mess;
				mListView.getAdapter().notifyItemChanged(mPluginList.indexOf(plugin));
			}
		} else if (TopicType.Gateway == topicType) {
			mToolbar.setSubtitle(topic.gateway() + " " + mess);

			mCallback.onGateway(topic.gateway());
		}

		if (mPluginList.size() > 0) {
			mProgressBar.setVisibility(View.GONE);
		}
	}

	private Plugin findByName(String name) {
		if (mPluginList == null) {
			return null;
		}

		for (Plugin plugin : mPluginList) {
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

	@Override
	public void onClick(View v) {
		getFragmentManager().popBackStack();
	}

	class Adapter extends RecyclerView.Adapter<Holder> implements View.OnClickListener {

		@Override
		public Holder onCreateViewHolder(ViewGroup parent, int viewType) {
			return new Holder(LayoutInflater.from(getActivity()).inflate(R.layout.v_plugin_item, parent, false));
		}

		@Override
		public void onBindViewHolder(Holder holder, int position) {
			Plugin plugin = mPluginList.get(position);

			holder.name.setText(plugin.name);
			holder.status.setText(plugin.status);

			holder.itemView.setTag(plugin);
			holder.itemView.setOnClickListener(this);
		}

		@Override
		public int getItemCount() {
			return mPluginList.size();
		}

		@Override
		public void onClick(View view) {
			Plugin plugin = (Plugin) view.getTag();

			getActivity().getSupportFragmentManager()
					.beginTransaction()
					.setCustomAnimations(
							R.anim.enter_from_right, R.anim.exit_to_left,
							R.anim.enter_from_left, R.anim.exit_to_right
					)
					.replace(R.id.content, PluginFragment.newInstance(plugin))
					.addToBackStack(null)
					.commit();
		}
	}

	class Holder extends RecyclerView.ViewHolder {
		final TextView name;
		final TextView status;

		public Holder(View itemView) {
			super(itemView);

			name = (TextView) itemView.findViewById(R.id.name);
			status = (TextView) itemView.findViewById(R.id.status);
		}
	}
}