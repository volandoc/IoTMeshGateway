package com.globallogic.gl_smart.ui.fragments;

import android.os.Bundle;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.support.v7.widget.SwitchCompat;
import android.text.TextUtils;
import android.util.Log;
import android.view.KeyEvent;
import android.view.View;
import android.view.ViewGroup;
import android.view.inputmethod.EditorInfo;
import android.widget.AdapterView;
import android.widget.CompoundButton;
import android.widget.EditText;
import android.widget.Spinner;
import android.widget.TextView;

import com.globallogic.gl_smart.App;
import com.globallogic.gl_smart.R;
import com.globallogic.gl_smart.model.mqtt.Capability;
import com.globallogic.gl_smart.model.mqtt.Property;
import com.globallogic.gl_smart.model.mqtt.PropertyPayload;
import com.globallogic.gl_smart.model.mqtt.StatusMessage;
import com.globallogic.gl_smart.model.mqtt.Topic;
import com.globallogic.gl_smart.model.type.AccessRight;
import com.globallogic.gl_smart.model.type.LimitationType;
import com.globallogic.gl_smart.model.type.MessageType;
import com.globallogic.gl_smart.model.type.PropertyType;
import com.globallogic.gl_smart.ui.base.BaseFragment;
import com.globallogic.gl_smart.ui.view.AppSeekBar;
import com.globallogic.gl_smart.ui.view.AppSpinnerAdapter;
import com.globallogic.gl_smart.utils.MqttManager;
import com.globallogic.gl_smart.utils.Utils;
import com.google.gson.JsonArray;
import com.google.gson.JsonElement;

import org.eclipse.paho.client.mqttv3.IMqttDeliveryToken;
import org.eclipse.paho.client.mqttv3.MqttCallback;
import org.eclipse.paho.client.mqttv3.MqttMessage;

import java.util.ArrayList;
import java.util.List;

import static com.globallogic.gl_smart.ui.fragments.GatewayFragment.property;

/**
 * @author eugenii.samarskyi.
 */
public abstract class NodeFragment extends BaseFragment implements TextView.OnEditorActionListener,
		CompoundButton.OnCheckedChangeListener, AdapterView.OnItemSelectedListener, AppSeekBar.Callback, MqttCallback {

	private static final String TAG = NodeFragment.class.getSimpleName();

	protected RecyclerView mListView;

	protected List<Property> mProperties = new ArrayList<>();
	protected List<Capability> mCapabilities;

	protected abstract String getTopic();

	@Override
	public void onViewCreated(View view, Bundle savedInstanceState) {
		if (mListView == null) {
			mListView = (RecyclerView) view.findViewById(R.id.list);
			mListView.setLayoutManager(new LinearLayoutManager(getActivity()));
		}
	}

	@Override
	public void onResume() {
		super.onResume();

		Log.d(TAG, "onResume: ");
		MqttManager.self().subscribe(getTopic(), this);
	}

	@Override
	public void onDestroy() {
		Log.d(TAG, "onDestroy: ");

		MqttManager.self().unSubscribe(getTopic());

		super.onDestroy();
	}

	protected void onStatus(Topic topic, StatusMessage message) {
		mCapabilities = message.data;
		mListView.setAdapter(new Adapter());

		for (Capability capability : mCapabilities) {
			Topic propertyTopic = new Topic.Builder()
					.gatewayId(topic.gateway())
					.type(MessageType.Property)
					.property(capability.name)
					.build();

			MqttManager.self().subscribe(propertyTopic.topic, this);

			//TODO just for test
			App.getHandler().postDelayed(new Runnable() {
				@Override
				public void run() {
					try {
						messageArrived("A000000000000001/SSIDPassword", new MqttMessage(property.getBytes()));
					} catch (Exception e) {
						e.printStackTrace();
					}
				}
			}, 2000);
		}
	}

	protected void onCommand() {
	}

	protected void onEvent() {
	}

	protected void onProperty(Property property) {
		Log.d(TAG, "onProperty: " + property.name + " " + property.value);

		Property p = getProperty(property.name);
		if (p == null) {
			mProperties.add(property);
			mListView.getAdapter().notifyItemChanged(getCapabilityIndex(property.name));
		} else {
			p.value = property.value;
			mListView.getAdapter().notifyItemChanged(getCapabilityIndex(p.name));
		}
	}

	@Override
	public void messageArrived(String topic, MqttMessage message) throws Exception {
		String mess = new String(message.getPayload());
		Log.d(TAG, "Topic: " + topic + ", Message: " + mess);

		Topic t = new Topic(topic);

		switch (t.getMessageType()) {
			case Status:
				onStatus(t, App.getGson().fromJson(mess, StatusMessage.class));
				break;
			case Property:
				Property property = new Property();

				// get property name from topic
				String[] arr = topic.split(Utils.SEPARATOR);
				property.name = arr[arr.length - 1];

				// get property value from payload
				PropertyPayload payload = new PropertyPayload(mess);
				property.value = payload.getValue();

				onProperty(property);
				break;
			case Command:
				onCommand();
				break;
			case Event:
				onEvent();
				break;
		}
	}

	public Property getProperty(String name) {
		for (Property property : mProperties) {
			if (property.name.equals(name)) {
				return property;
			}
		}

		return null;
	}

	public int getCapabilityIndex(String name) {
		for (Capability capability : mCapabilities) {
			if (capability.name.equals(name)) {
				return mCapabilities.indexOf(capability);
			}
		}

		return 0;
	}

	@Override
	public void deliveryComplete(IMqttDeliveryToken token) {
		Log.d(TAG, "Delivery Complete!");
	}

	@Override
	public void connectionLost(Throwable cause) {
//		Log.v(TAG, "Connection was lost");
	}

	private enum HolderType {
		RANGE, LIST, ENUM, TEXT, BOOLEAN, NUMBER, URL;

		public static HolderType fromCapability(Capability capability) {
			LimitationType constraintType = LimitationType.fromString(capability.lim_type);
			PropertyType dataType = PropertyType.fromString(capability.type);

			switch (constraintType) {
				case Range:
					return RANGE;

				case Enum:
					return ENUM;

				case List:
					return LIST;

				case Null:
					switch (dataType) {
						case Bool:
							return BOOLEAN;
						case Float:
						case Int:
							return NUMBER;
						case Url:
							return URL;

						default:
							return TEXT;
					}
				default:
					return TEXT;
			}
		}

		public static HolderType fromInt(int ordinal) {
			for (HolderType type : HolderType.values()) {
				if (type.ordinal() == ordinal) {
					return type;
				}
			}

			return HolderType.TEXT;
		}
	}

	class Adapter extends RecyclerView.Adapter<Holder> {

		private final int ITEM_LAYOUT_ID = R.layout.v_property_item;

		@Override
		public Holder onCreateViewHolder(ViewGroup parent, int viewType) {
			HolderType type = HolderType.fromInt(viewType);
			View itemView = Utils.inflate(parent, ITEM_LAYOUT_ID);

			switch (type) {
				case BOOLEAN:
					return new BoolHolder(itemView);
				case RANGE:
					return new RangeHolder(itemView);
				case ENUM:
					return new EnumHolder(itemView);
				case TEXT:
					return new TextHolder(itemView);
//				case NUMBER:
//					return new NumberHolder(itemView);
				default:
					return new TextHolder(itemView);
			}
		}

		@Override
		public void onBindViewHolder(Holder holder, int position) {
			holder.bindView(position);
		}

		@Override
		public int getItemCount() {
			return mCapabilities.size();
		}

		@Override
		public int getItemViewType(int position) {
			return HolderType.fromCapability(mCapabilities.get(position)).ordinal();
		}
	}

	private abstract class Holder extends RecyclerView.ViewHolder {

		public final View value;
		public final TextView name;
		public final ViewGroup valueContainer;

		public Holder(View itemView) {
			super(itemView);

			name = (TextView) itemView.findViewById(R.id.name);
			valueContainer = (ViewGroup) itemView.findViewById(R.id.value_container);
			Utils.inflate(valueContainer, valueLayout(), true);
			value = valueContainer.getChildAt(0);
		}

		public void bindView(int position) {
			name.setText(mCapabilities.get(position).name);

			boolean enabled = !AccessRight.fromString(mCapabilities.get(position).rw).isReadOnly();
			Utils.setEnabledHierarchically(value, enabled);
		}

		protected abstract int valueLayout();
	}

	private class TextHolder extends Holder {

		TextHolder(View itemView) {
			super(itemView);
		}

		@Override
		protected int valueLayout() {
			return R.layout.v_text;
		}

		@Override
		public void bindView(final int position) {
			super.bindView(position);

			final EditText valueText = (EditText) value;
			final String value;

			Property property = getProperty(mCapabilities.get(position).name);
			if (property != null) {
				value = property.value.toString();
			} else {
				value = TextUtils.isEmpty(mCapabilities.get(position).def) ? "" : mCapabilities.get(position).def;
			}

			valueText.setText(value);
			valueText.setTag(mCapabilities.get(position));
			valueText.setSingleLine();
			valueText.setImeOptions(EditorInfo.IME_ACTION_DONE);
			valueText.setOnEditorActionListener(NodeFragment.this);
		}
	}

	private class BoolHolder extends Holder {

		public BoolHolder(View itemView) {
			super(itemView);
		}

		@Override
		protected int valueLayout() {
			return R.layout.v_switch;
		}

		@Override
		public void bindView(int position) {
			super.bindView(position);

			SwitchCompat valueText = (SwitchCompat) value;
			valueText.setChecked(Boolean.valueOf(mCapabilities.get(position).def));
			valueText.setTag(mCapabilities.get(position));
			valueText.setOnCheckedChangeListener(NodeFragment.this);
		}
	}

	private class EnumHolder extends Holder {

		public EnumHolder(View itemView) {
			super(itemView);
		}

		@Override
		protected int valueLayout() {
			return R.layout.v_spinner;
		}

		@Override
		public void bindView(int position) {
			super.bindView(position);

			List<String> values = getRangeFromJsonArray(mCapabilities.get(position).getLimitation());
//			final int currentIndex = mPropertyList.get(position).getCurrentConstraintPosition();

			Spinner view = (Spinner) value;

			AppSpinnerAdapter<String> adapter = new AppSpinnerAdapter<>(getContext(), values, new AppSpinnerAdapter.AppSpinnerAdapterCallback<String>() {
				@Override
				public String getText(String item) {
					return item;
				}

				@Override
				public boolean isEnabled(String item) {
					return true;
				}
			});

			adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
			view.setAdapter(adapter);
			view.setTag(mCapabilities.get(position));
//			view.setSelection(currentIndex);
			view.setOnItemSelectedListener(NodeFragment.this);
		}
	}

	private class RangeHolder extends Holder {

		public RangeHolder(View itemView) {
			super(itemView);
		}

		@Override
		protected int valueLayout() {
			return R.layout.v_range;
		}

		@Override
		public void bindView(final int position) {
			super.bindView(position);

			AppSeekBar rangeBar = (AppSeekBar) value.findViewById(R.id.range_seek_bar);
			TextView currentValueView = (TextView) value.findViewById(R.id.range_current_value);
			currentValueView.setText(mCapabilities.get(position).def);

//			try {
//				UiUtils.setWithProperty(rangeBar, currentValueView, mPropertyList.get(position));
//			} catch (NumberFormatException e) {
//				Log.e(TAG, "bindView: ", e);
//			}
//
			rangeBar.setTag(mCapabilities.get(position));
			rangeBar.setOnSeekBarChangeListener(NodeFragment.this);
		}
	}

	@Override
	public boolean onEditorAction(TextView v, int actionId, KeyEvent event) {
		return false;
	}

	@Override
	public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {

	}

	@Override
	public void onItemSelected(AdapterView<?> parent, View view, int position, long id) {

	}

	@Override
	public void onNothingSelected(AdapterView<?> parent) {

	}

	@Override
	public void onStopTrackingTouch(AppSeekBar seekBar) {

	}

	private List<String> getRangeFromJsonArray(JsonArray array) {
		List<String> list = new ArrayList<>();

		for (JsonElement jsonElement : array) {
			list.add(jsonElement.getAsString());
		}

		return list;
	}
}
