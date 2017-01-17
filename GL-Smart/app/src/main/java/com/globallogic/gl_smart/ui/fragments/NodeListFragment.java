package com.globallogic.gl_smart.ui.fragments;

import android.content.Context;
import android.os.Bundle;
import android.support.annotation.StringRes;
import android.support.v4.app.Fragment;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.support.v7.widget.Toolbar;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ProgressBar;
import android.widget.TextView;

import com.globallogic.gl_smart.App;
import com.globallogic.gl_smart.BuildConfig;
import com.globallogic.gl_smart.R;
import com.globallogic.gl_smart.model.Node;
import com.globallogic.gl_smart.model.Plugin;
import com.globallogic.gl_smart.model.Sensor;
import com.globallogic.gl_smart.model.mqtt.StatusMessage;
import com.globallogic.gl_smart.model.mqtt.Topic;
import com.globallogic.gl_smart.model.type.TopicType;
import com.globallogic.gl_smart.ui.GatewayCallback;
import com.globallogic.gl_smart.ui.MainActivity;
import com.globallogic.gl_smart.ui.NodeActivity;

import org.eclipse.paho.client.mqttv3.MqttMessage;

import java.util.ArrayList;
import java.util.List;

import static com.globallogic.gl_smart.ui.fragments.GatewayFragment.pluginStatus;
import static com.globallogic.gl_smart.ui.fragments.GatewayFragment.sensorStatus;
import static com.globallogic.gl_smart.ui.fragments.GatewayFragment.status;

/**
 * @author eugenii.samarskyi.
 */
public class NodeListFragment extends MqttFragment {
	private static final String TAG = NodeListFragment.class.getSimpleName();

	protected List<Node> mData;

	private Toolbar mToolbar;
	protected RecyclerView mListView;
	protected ProgressBar mProgressBar;

	private GatewayCallback mCallback;

	private TopicType mTopicType;

	public static Fragment newInstance(String[] topics, @StringRes int res, TopicType type) {
		Bundle args = new Bundle();
		args.putStringArray("topics", topics);
		args.putInt("title", res);
		args.putSerializable("type", type);

		Fragment fragment = new NodeListFragment();
		fragment.setArguments(args);

		return fragment;
	}

	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

		mData = new ArrayList<>();
		mTopicType = (TopicType) getArguments().getSerializable("type");
	}

	@Override
	public void onAttach(Context context) {
		super.onAttach(context);

		mCallback = (GatewayCallback) context;

		if (context instanceof MainActivity) {
			mToolbar = ((MainActivity) context).getToolbar();
			mToolbar.setTitle(null);
			mToolbar.setSubtitle(null);
			mToolbar.getMenu().clear();
		}
	}

	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
		return inflater.inflate(R.layout.f_node_list, container, false);
	}

	@Override
	public void onViewCreated(View view, Bundle savedInstanceState) {
		mListView = (RecyclerView) view.findViewById(R.id.list);
		mListView.setLayoutManager(new LinearLayoutManager(getActivity()));
		mListView.setAdapter(new Adapter());

		mToolbar.setTitle(getArguments().getInt("title"));

		mProgressBar = (ProgressBar) view.findViewById(R.id.progress);

		if (BuildConfig.BUILD_TYPE.equals("offlane")) {
			App.getHandler().postDelayed(new Runnable() {
				@Override
				public void run() {
					try {
						messageArrived(pluginStatus, new MqttMessage(status.getBytes()));
						messageArrived(sensorStatus, new MqttMessage(status.getBytes()));
					} catch (Exception e) {
						Log.e(TAG, "run: " + e.getLocalizedMessage());
					}
				}
			}, 2000);
		}
	}

	@Override
	protected String[] getTopics() {
		return getArguments().getStringArray("topics");
	}

	@Override
	protected void onStatus(Topic topic, StatusMessage message) {
		TopicType topicType = TopicType.fromString(topic.topic);
		if (TopicType.Gateway == topicType) {
			mToolbar.setSubtitle(topic.gateway() + " " + message.status);
			mCallback.onGateway(topic.gateway());
		} else {
			if (topicType != mTopicType) {
				return;
			}
			String nodeName = topic.get(topicType);
			Node node = findByName(nodeName);
			if (node == null) {
				if (TopicType.Sensor == topicType) {
					mData.add(new Sensor(nodeName, message.status, topic.gateway(), topic.plugin()));
				} else if (TopicType.Plugin == topicType) {
					mData.add(new Plugin(nodeName, message.status, topic.gateway()));
				}
				mListView.getAdapter().notifyItemInserted(mData.size() - 1);
			} else {
				node.status = message.status;
				mListView.getAdapter().notifyItemChanged(mData.indexOf(node));
			}

			if (mData.size() > 0) {
				mProgressBar.setVisibility(View.GONE);
			}
		}
	}

	private class Adapter extends RecyclerView.Adapter<Holder> implements View.OnClickListener {

		@Override
		public Holder onCreateViewHolder(ViewGroup parent, int viewType) {
			return new Holder(LayoutInflater.from(getActivity()).inflate(R.layout.v_node_item, parent, false));
		}

		@Override
		public void onBindViewHolder(Holder holder, int position) {
			Node node = mData.get(position);

			holder.name.setText(node.name);
			holder.status.setText(node.status);

			holder.itemView.setTag(node);
			holder.itemView.setOnClickListener(this);
		}

		@Override
		public int getItemCount() {
			return mData.size();
		}

		@Override
		public void onClick(View view) {
			startActivity(NodeActivity.newInstance((Node) view.getTag()));
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

	private Node findByName(String name) {
		if (mData == null) {
			return null;
		}

		for (Node node : mData) {
			if (node.name.equals(name)) {
				return node;
			}
		}

		return null;
	}
}
