package com.globallogic.gl_smart.ui.fragments;

import android.os.Bundle;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.support.v7.widget.SwitchCompat;
import android.text.Html;
import android.text.TextUtils;
import android.text.method.LinkMovementMethod;
import android.util.Log;
import android.view.KeyEvent;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.inputmethod.EditorInfo;
import android.widget.AdapterView;
import android.widget.CompoundButton;
import android.widget.Spinner;
import android.widget.TextView;

import com.globallogic.gl_smart.App;
import com.globallogic.gl_smart.BuildConfig;
import com.globallogic.gl_smart.R;
import com.globallogic.gl_smart.model.Node;
import com.globallogic.gl_smart.model.mqtt.Capability;
import com.globallogic.gl_smart.model.mqtt.Property;
import com.globallogic.gl_smart.model.mqtt.PropertyMessage;
import com.globallogic.gl_smart.model.mqtt.StatusMessage;
import com.globallogic.gl_smart.model.mqtt.Topic;
import com.globallogic.gl_smart.model.type.AccessRight;
import com.globallogic.gl_smart.model.type.LimitationType;
import com.globallogic.gl_smart.model.type.MessageType;
import com.globallogic.gl_smart.model.type.PropertyType;
import com.globallogic.gl_smart.ui.view.AppSeekBar;
import com.globallogic.gl_smart.ui.view.AppSpinnerAdapter;
import com.globallogic.gl_smart.utils.MqttManager;
import com.globallogic.gl_smart.utils.Utils;
import com.google.gson.JsonArray;
import com.google.gson.JsonElement;

import org.eclipse.paho.client.mqttv3.MqttMessage;

import java.util.ArrayList;
import java.util.List;
import java.util.regex.Pattern;

import static com.globallogic.gl_smart.ui.fragments.GatewayFragment.property;

/**
 * @author eugenii.samarskyi.
 */
public abstract class NodeFragment extends MqttFragment implements TextView.OnEditorActionListener,
		CompoundButton.OnCheckedChangeListener, AdapterView.OnItemSelectedListener, AppSeekBar.Callback {

	private static final String TAG = NodeFragment.class.getSimpleName();

	protected RecyclerView mListView;

	protected Node mNode;
	protected Topic mTopic;
	protected List<Capability> mCapabilities;
	protected List<Property> mProperties = new ArrayList<>();

	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
		return inflater.inflate(R.layout.f_node, container, false);
	}

	@Override
	public void onViewCreated(View view, Bundle savedInstanceState) {
		if (mListView == null) {
			mListView = (RecyclerView) view.findViewById(R.id.list);
			mListView.setLayoutManager(new LinearLayoutManager(getActivity()));
		}
	}

	protected void onCommand() {
	}

	protected void onEvent() {
	}

	protected void onStatus(Topic topic, StatusMessage message) {
		mTopic = topic;

		mCapabilities = message.data;
		mListView.setAdapter(new Adapter());

		mProperties.clear();
		for (Capability capability : mCapabilities) {
			Property p = new Property();
			p.name = capability.name;
			mProperties.add(p);

			Topic propertyTopic = new Topic.Builder()
					.gatewayId(topic.gateway())
					.type(MessageType.Property)
					.property(capability.name)
					.build();

			MqttManager.self().subscribe(propertyTopic.topic, this);
		}

		if (BuildConfig.BUILD_TYPE.equals("offlane")) {
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

	protected void onProperty(Property property) {
		Log.d(TAG, "onProperty: " + property.name + " " + property.value);

		Property p = getProperty(property.name);
		p.value = property.value;
		mListView.getAdapter().notifyItemChanged(getCapabilityIndex(p.name));
	}

	protected void changeProperty(Property p) {
		PropertyMessage message = new PropertyMessage(p);

		Topic t = new Topic.Builder(mTopic).type(MessageType.Command).build();
		Log.i(TAG, "ChangeProperty\nTopic = " + t.topic + "\nMessage  = " + message.getAsJson());
		MqttManager.self().sendMessage(t.topic, message.getAsJson());
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

	private class Adapter extends RecyclerView.Adapter<Holder> {

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
				case URL:
					return new UrlHolder(itemView);
				case NUMBER:
					return new NumberHolder(itemView);
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

		TextView valueText;

		TextHolder(View itemView) {
			super(itemView);
		}

		@Override
		protected int valueLayout() {
			return R.layout.v_input_text;
		}

		@Override
		public void bindView(final int position) {
			super.bindView(position);

			valueText = (TextView) value;
			final String value;

			Property property = getProperty(mCapabilities.get(position).name);
			if (property.value != null) {
				value = property.value.toString();
			} else {
				value = TextUtils.isEmpty(mCapabilities.get(position).def) ? "" : mCapabilities.get(position).def;
			}

			PropertyType type = PropertyType.fromString(mCapabilities.get(position).type);

			if (PropertyType.Url == type) {
				valueText.setText(Html.fromHtml(String.format(getString(R.string.link_format), value, value)));
				valueText.setEnabled(true);
				valueText.setMovementMethod(LinkMovementMethod.getInstance());
			} else {
				valueText.setText(value);
				valueText.setTag(mCapabilities.get(position));
				valueText.setSingleLine();
				valueText.setImeOptions(EditorInfo.IME_ACTION_DONE);
				valueText.setOnEditorActionListener(NodeFragment.this);
			}

			Utils.setWithDataType(valueText, type);
		}
	}

	private class UrlHolder extends TextHolder {

		UrlHolder(View itemView) {
			super(itemView);
		}

		@Override
		protected int valueLayout() {
			return R.layout.v_text;
		}

		@Override
		public void bindView(final int position) {
			super.bindView(position);
		}
	}

	private class NumberHolder extends TextHolder {

		NumberHolder(View itemView) {
			super(itemView);
		}

		@Override
		protected int valueLayout() {
			return R.layout.v_input_text;
		}

		@Override
		public void bindView(final int position) {
			super.bindView(position);
		}
	}

	private class BoolHolder extends Holder {

		BoolHolder(View itemView) {
			super(itemView);
		}

		@Override
		protected int valueLayout() {
			return R.layout.v_switch;
		}

		@Override
		public void bindView(int position) {
			super.bindView(position);

			SwitchCompat switchView = (SwitchCompat) value;

			Property property = getProperty(mCapabilities.get(position).name);
			if (property.value != null) {
				switchView.setChecked((boolean) (property.value));
			} else {
				switchView.setChecked(
						TextUtils.isEmpty(mCapabilities.get(position).def)
								? false
								: Boolean.valueOf(mCapabilities.get(position).def));
			}

			switchView.setTag(mCapabilities.get(position));
			switchView.setOnCheckedChangeListener(NodeFragment.this);
		}
	}

	private class EnumHolder extends Holder {

		EnumHolder(View itemView) {
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

			int index = 0;
			Object v = getProperty(mCapabilities.get(position).name).value;
			if (v != null) {
				index = values.indexOf(String.valueOf(v));
			} else if (mCapabilities.get(position).def != null) {
				index = values.indexOf(mCapabilities.get(position).def);
			}

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
			view.setSelection(index);
			view.setOnItemSelectedListener(NodeFragment.this);
		}
	}

	private class RangeHolder extends Holder {

		RangeHolder(View itemView) {
			super(itemView);
		}

		@Override
		protected int valueLayout() {
			return R.layout.v_range;
		}

		@Override
		public void bindView(final int position) {
			super.bindView(position);

			Capability capability = mCapabilities.get(position);

			AppSeekBar rangeBar = (AppSeekBar) value.findViewById(R.id.range_seek_bar);
			TextView currentValueView = (TextView) value.findViewById(R.id.range_current_value);

			try {
				Utils.setWithProperty(rangeBar, currentValueView, capability, capability.def);
			} catch (NumberFormatException e) {
				Log.e(TAG, "bindView: ", e);
			}

			rangeBar.setTag(mCapabilities.get(position));
			rangeBar.setOnSeekBarChangeListener(NodeFragment.this);
		}
	}

	@Override
	public boolean onEditorAction(TextView v, int actionId, KeyEvent event) {
		if (actionId == EditorInfo.IME_ACTION_DONE) {
			String newText = v.getText().toString();

			if (TextUtils.isEmpty(newText)) {
				v.setError(getString(R.string.error_empty_field));
				return true;
			}

			Capability capability = (Capability) v.getTag();
			if (LimitationType.Regexp == LimitationType.fromString(capability.lim_type)) {
				String regexp = capability.getLimitation().get(0).getAsString();

				if (!Pattern.compile(regexp).matcher(newText).matches()) {
					v.setError(getString(R.string.error_wrong_regexp));
					return true;
				}
			}

			Utils.hideSoftKeyboard(v);

			Property property = new Property();
			property.name = capability.name;
			property.value = newText;

			changeProperty(property);

			return true;
		}
		return false;
	}

	@Override
	public void onCheckedChanged(CompoundButton v, boolean isChecked) {
		// not manual change event
		if (!v.isPressed()) {
			return;
		}

		Capability capability = (Capability) v.getTag();

		Property property = new Property();
		property.name = capability.name;
		property.value = isChecked;

		changeProperty(property);
	}

	@Override
	public void onItemSelected(AdapterView<?> parent, View view, int position, long id) {
		Capability capability = (Capability) parent.getTag();

		if (AccessRight.fromString(capability.rw).isReadOnly()) {
			return;
		}

		// not manual event
		if (getProperty(capability.name).value == null && capability.def != null) {
			getProperty(capability.name).value = capability.def;
			return;
		}

		if (parent.getSelectedItem().toString().equals(getProperty(capability.name).value)) {
			return;
		}

		Property property = new Property();
		property.name = capability.name;
		property.value = parent.getSelectedItem().toString();

		changeProperty(property);
	}

	@Override
	public void onNothingSelected(AdapterView<?> parent) {

	}

	@Override
	public void onStopTrackingTouch(AppSeekBar seekBar) {
		Capability capability = (Capability) seekBar.getTag();

		Property property = new Property();
		property.name = capability.name;
		property.value = seekBar.getValue();

		changeProperty(property);
	}

	private List<String> getRangeFromJsonArray(JsonArray array) {
		List<String> list = new ArrayList<>();

		for (JsonElement jsonElement : array) {
			list.add(jsonElement.getAsString());
		}

		return list;
	}
}
