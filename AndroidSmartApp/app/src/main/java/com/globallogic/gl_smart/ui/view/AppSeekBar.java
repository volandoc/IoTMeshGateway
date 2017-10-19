package com.globallogic.gl_smart.ui.view;

import android.content.Context;
import android.util.AttributeSet;
import android.widget.LinearLayout;
import android.widget.SeekBar;
import android.widget.TextView;

import com.globallogic.gl_smart.R;

import java.util.Locale;

/**
 * @author eugenii.samarskyi.
 */
public class AppSeekBar extends LinearLayout implements SeekBar.OnSeekBarChangeListener {

	public interface Callback {
		void onStopTrackingTouch(AppSeekBar seekBar);
	}

	private SeekBar mSeekBar;
	private TextView mMinView;
	private TextView mMaxView;
	private TextView mCurrentValue;
	private Callback mCallback;
	private Double mShift;
	private boolean mDecimalValue;
	private String mValueFormat;
	private double mStep = 1;
	private String mRawData;

	public AppSeekBar(Context context) {
		super(context);

		inflate(context, R.layout.v_seekbar, this);

		init();
	}

	public AppSeekBar(Context context, AttributeSet attrs) {
		super(context, attrs);

		inflate(context, R.layout.v_seekbar, this);

		init();
	}

	public void setCurrentValueView(TextView view) {
		mCurrentValue = view;
	}

	public void setShift(double shift) {
		mShift = shift;
	}

	public void setTag(Object obj) {
		mSeekBar.setTag(obj);
	}

	public Object getTag() {
		return mSeekBar.getTag();
	}

	public void setProgress(double progress) {
		mSeekBar.setProgress(getRealValue(progress));
		updateCurrentValue();
	}

	public void setDecimal(boolean useDecimal) {
		mDecimalValue = useDecimal;
	}

	public void setStep(String step) {
		double dStep = Double.parseDouble(step);
		if (mDecimalValue) {
			mStep = 1 / dStep;
			int dot = step.indexOf('.');
			mValueFormat = "%." + (dot != -1 ? (step.length() - dot - 1) : 0) + "f";
		} else {
			mStep = (int) dStep;
			mValueFormat = "%.0f";
		}
	}

	public void setMax(double max) {
		mSeekBar.setMax(getRealValue(max));
	}

	public void setMinValue(Double min) {
		mMinView.setText(mDecimalValue ? String.valueOf(min) : String.valueOf(min.intValue()));
	}

	public void setMaxValue(Double max) {
		mMaxView.setText(mDecimalValue ? String.valueOf(max) : String.valueOf(max.intValue()));
	}

	public void setOnSeekBarChangeListener(Callback listener) {
		mCallback = listener;
	}

	private void init() {
		mSeekBar = (SeekBar) getChildAt(1);
		mSeekBar.setOnSeekBarChangeListener(this);

		mMinView = (TextView) getChildAt(0);
		mMaxView = (TextView) getChildAt(2);
	}

	@Override
	public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
		updateCurrentValue();
	}

	@Override
	public void onStartTrackingTouch(SeekBar seekBar) {

	}

	@Override
	public void onStopTrackingTouch(SeekBar seekBar) {
		if (mCallback != null) {
			mCallback.onStopTrackingTouch(this);
			updateCurrentValue();
		}
	}

	private void updateCurrentValue() {
		if (mCurrentValue != null) {
			mCurrentValue.setText(mRawData == null ? getStringValue() : mRawData);
		}
	}

	public String getStringValue() {
		Double value = getRealValue(mSeekBar.getProgress());

		return String.format(Locale.ENGLISH, mValueFormat, value);
	}

	public Number getValue() {
		if (mDecimalValue) {
			return getRealValue(mSeekBar.getProgress());
		} else {
			return (int) getRealValue(mSeekBar.getProgress());
		}
	}

	private int getRealValue(double value) {
		return (int) ((value - mShift) * mStep);
	}

	private double getRealValue(int value) {
		return value / mStep + mShift;
	}

	public void setRawData(String rawData) {
		mRawData = rawData;
	}
}
