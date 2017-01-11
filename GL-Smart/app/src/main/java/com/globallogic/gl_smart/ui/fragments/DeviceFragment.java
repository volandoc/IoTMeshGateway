package com.globallogic.gl_smart.ui.fragments;

import android.os.Bundle;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.support.v7.widget.SwitchCompat;
import android.view.KeyEvent;
import android.view.View;
import android.view.ViewGroup;
import android.view.inputmethod.EditorInfo;
import android.widget.AdapterView;
import android.widget.CompoundButton;
import android.widget.EditText;
import android.widget.Spinner;
import android.widget.TextView;

import com.globallogic.gl_smart.R;
import com.globallogic.gl_smart.model.mqtt.Capability;
import com.globallogic.gl_smart.model.mqtt.Property;
import com.globallogic.gl_smart.model.type.AccessRight;
import com.globallogic.gl_smart.model.type.LimitationType;
import com.globallogic.gl_smart.model.type.PropertyType;
import com.globallogic.gl_smart.ui.base.BaseFragment;
import com.globallogic.gl_smart.ui.view.AppSeekBar;
import com.globallogic.gl_smart.ui.view.AppSpinnerAdapter;
import com.globallogic.gl_smart.utils.Utils;
import com.google.gson.JsonArray;
import com.google.gson.JsonElement;

import java.util.ArrayList;
import java.util.List;

/**
 * Created by Batman on 11.01.2017.
 */

public class DeviceFragment extends BaseFragment implements TextView.OnEditorActionListener,
		CompoundButton.OnCheckedChangeListener, AdapterView.OnItemSelectedListener, AppSeekBar.Callback {

	private static final String TAG = DeviceFragment.class.getSimpleName();

	protected RecyclerView mListView;

	protected List<Property> mProperties;
	protected List<Capability> mCapabilities;

	@Override
	public void onViewCreated(View view, Bundle savedInstanceState) {
		mListView = (RecyclerView) view.findViewById(R.id.list);
		mListView.setLayoutManager(new LinearLayoutManager(getActivity()));
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

	protected class Adapter extends RecyclerView.Adapter<Holder> {

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

			// link should be always enabled
			final String value = mCapabilities.get(position).def;

			valueText.setText(value);
			valueText.setTag(mCapabilities.get(position));
			valueText.setSingleLine();
			valueText.setImeOptions(EditorInfo.IME_ACTION_DONE);
			valueText.setOnEditorActionListener(DeviceFragment.this);
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
			valueText.setOnCheckedChangeListener(DeviceFragment.this);
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

			List<String> values = getRangeFromJsonArray(mCapabilities.get(position).lim_json);
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
			view.setOnItemSelectedListener(DeviceFragment.this);
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
			rangeBar.setOnSeekBarChangeListener(DeviceFragment.this);
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
