package com.globallogic.gl_smart.ui.fragments;

import android.graphics.PorterDuff;
import android.graphics.drawable.Drawable;
import android.os.Bundle;
import android.support.v4.content.ContextCompat;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.support.v7.widget.Toolbar;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.MenuItem;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ProgressBar;
import android.widget.TextView;

import com.globallogic.gl_smart.R;
import com.globallogic.gl_smart.model.Plugin;
import com.globallogic.gl_smart.model.mqtt.Topic;
import com.globallogic.gl_smart.model.mqtt.type.MessageType;
import com.globallogic.gl_smart.model.mqtt.type.SenderType;
import com.globallogic.gl_smart.ui.base.BaseFragment;
import com.globallogic.gl_smart.utils.MqttManager;

import org.eclipse.paho.client.mqttv3.IMqttDeliveryToken;
import org.eclipse.paho.client.mqttv3.MqttCallback;
import org.eclipse.paho.client.mqttv3.MqttMessage;

import java.util.ArrayList;
import java.util.List;

import static com.globallogic.gl_smart.R.id.toolbar;

/**
 * @author eugenii.samarskyi.
 */
public class GatewayFragment extends BaseFragment implements MqttCallback, Toolbar.OnMenuItemClickListener {

	private static final String TAG = GatewayFragment.class.getSimpleName();

	public static GatewayFragment newInstance() {
		return new GatewayFragment();
	}

	private Toolbar mToolbar;
	private RecyclerView mListView;
	private ProgressBar mProgressBar;

	private List<Plugin> mPluginList;

	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
		return inflater.inflate(R.layout.f_gateway, container, false);
	}

	@Override
	public void onViewCreated(View view, Bundle savedInstanceState) {
		mToolbar = (Toolbar) view.findViewById(toolbar);
		mToolbar.inflateMenu(R.menu.m_gateway);
		mToolbar.setOnMenuItemClickListener(this);

		mPluginList = new ArrayList<>();

		mListView = (RecyclerView) view.findViewById(R.id.list);
		mListView.setLayoutManager(new LinearLayoutManager(getActivity()));
		mListView.setAdapter(new Adapter());

		mProgressBar = (ProgressBar) view.findViewById(R.id.progress);
		mProgressBar.setVisibility(MqttManager.self().isConnected() ? View.VISIBLE : View.INVISIBLE);

		Drawable drawable = ContextCompat.getDrawable(getActivity(), R.drawable.ic_circle);
		drawable.setColorFilter(MqttManager.self().isConnected()
						? getResources().getColor(android.R.color.holo_green_light)
						: getResources().getColor(android.R.color.holo_red_light),
				PorterDuff.Mode.SRC_IN);

		mToolbar.setNavigationIcon(drawable);

		MqttManager.self().subscribe(this);
	}

	@Override
	public void connectionLost(Throwable cause) {
//		Log.v(TAG, "Connection was lost");
	}

	@Override
	public void onDestroy() {
		MqttManager.self().unSubscribe();

		super.onDestroy();
	}

	@Override
	public void messageArrived(String topic_, MqttMessage message) throws Exception {
		String mess = new String(message.getPayload());
		Log.d(TAG, "Topic: " + topic_ + ", Message: " + mess);

		Topic topic = new Topic(topic_);
		MessageType messageType = topic.getMessageType();
		if (MessageType.Status != messageType) {
			return;
		}

		SenderType senderType = SenderType.fromString(topic.topic);
		if (SenderType.Plugin == senderType) {
			Plugin plugin = findByName(topic.plugin());
			if (plugin == null) {
				mPluginList.add(new Plugin(topic.gateway(), topic.plugin(), mess));
				mListView.getAdapter().notifyItemInserted(mPluginList.size() - 1);
			} else {
				plugin.status = mess;
				mListView.getAdapter().notifyItemChanged(mPluginList.indexOf(plugin));
			}
		} else if (SenderType.Gateway == senderType) {
			mToolbar.setSubtitle(topic.gateway() + " " + mess);
		}

		if (mPluginList.size() > 0 && mProgressBar.getVisibility() == View.VISIBLE) {
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
	public boolean onMenuItemClick(MenuItem item) {
		if (item.getItemId() == R.id.settings) {
			getActivity().getSupportFragmentManager()
					.beginTransaction()
					.setCustomAnimations(
							R.anim.enter_from_right, R.anim.exit_to_left,
							R.anim.enter_from_left, R.anim.exit_to_right
					)
					.replace(R.id.content, SettingsFragment.newInstance())
					.addToBackStack(null)
					.commit();
			return true;
		}

		return false;
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
